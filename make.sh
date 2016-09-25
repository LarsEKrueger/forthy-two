#! /bin/bash

function showHelp
{
  msg=$1
  if test "${msg}" != ""; then
    echo "make.sh: ${msg}"
  fi
  echo "make.sh -- Make script for this project"
  echo "USAGE: make.sh [option]"
  echo
  echo "Options:"
  echo "  -h | --help   -- Show help"
  echo "  -d | --debug  -- Build debug version"
  echo "  -c | --cmake  -- Force a cmake run"
  echo "  -f | --format -- Format the source code"
}

MAKESH=$(realpath $0)
SANDBOX=$(dirname $MAKESH)

BUILD_TYPE=Release
FORCECMAKE=false
FORMATSOURCE=false

# Parse command line
while [[ $# -gt 0 ]]; do
  opt="$1"
  case $opt in
      -h|--help)
      showHelp
      exit 1
      ;;
      -d|--debug)
      BUILD_TYPE=Debug
      ;;
      -c|--cmake)
      FORCECMAKE=true
      ;;
      -f|--format)
      FORMATSOURCE=true
      ;;
      -*)
      showHelp "Unknown option ${opt}"
      break
      ;;
      *)
      break
      ;;
  esac
  shift # past argument or value
done

# Setup variables
BUILD_DIR="${SANDBOX}/build/${BUILD_TYPE}"
INSTALL_DIR="${SANDBOX}/install"
CPUS=$(cat /proc/cpuinfo | grep '^processor' | wc -l)
JOBS=${JOBS:-${CPUS}}

# format the source
if test "${FORMATSOURCE}" == true; then
  cd ${SANDBOX}
  uncrustify -c uncrustify.cfg --replace $(find forth test apps -name '*.cpp' -o -name '*.hpp')
  exit 0
fi

if test "${FORCECMAKE}" == true; then
  rm -f "${BUILD_DIR}/CMakeCache.txt" "${BUILD_DIR}/Makefile"
fi

# Try to run cmake
if [ \! -f "${BUILD_DIR}/Makefile" ] ; then
  (
    mkdir -p ${BUILD_DIR}
    cd ${BUILD_DIR}
    cmake -G Ninja -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} ${SANDBOX}
  )
fi

cd ${BUILD_DIR}
export CTEST_OUTPUT_ON_FAILURE=1
ninja -j ${JOBS} install && ninja test

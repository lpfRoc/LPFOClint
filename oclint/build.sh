#! /usr/bin/env sh

if [ ! -d "build" ]; then
	cd oclint-scripts && ./make && cd -
fi

if [ -d "Lint-XCodeProject" ]; then
  rm -rf Lint-XCodeProject
fi

mkdir Lint-XCodeProject
cd Lint-XCodeProject
cmake -G Xcode \
  -D CMAKE_CXX_COMPILER=../build/llvm-install/bin/clang++  \
  -D CMAKE_C_COMPILER=../build/llvm-install/bin/clang \
  -D OCLINT_BUILD_DIR=../build/oclint-core \
  -D OCLINT_SOURCE_DIR=../oclint-core \
  -D OCLINT_METRICS_SOURCE_DIR=../oclint-metrics \
  -D OCLINT_METRICS_BUILD_DIR=../build/oclint-metrics \
  -D LLVM_ROOT=../build/llvm-install/ ../oclint-rules
cd -

CUSTOM_DIR="oclint-rules/rules/custom/*.cpp"
for file in $CUSTOM_DIR; do
    CPPFILE=${file##*/}
    TARGET=${CPPFILE%.*}
    echo "\033[32m#####找到规则文件${file},开始编译####\033[0m" 
    xcodebuild -project Lint\-XCodeProject/OCLINT_RULES.xcodeproj -target ${TARGET} -configuration Release | xcpretty
    DYLIBFILE="Lint-XCodeProject/rules.dl/Release/lib${TARGET}.dylib"
    if [ ! -f "$DYLIBFILE" ]; then
    	echo "\033[31mError:生成dylib文件失败,未找到$DYLIBFILE\033[0m"
    else
    	echo "\033[32m生成lib${TARGET}.dylib\033[0m" 
    	mv -f $DYLIBFILE build/oclint-release/lib/oclint/rules/
    fi
done

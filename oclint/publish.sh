#! /usr/bin/env sh

sh build.sh

TAR_FILE="oclint-release-$(date +%Y%m%d%H%M).tar.xz"

cd build && tar -jcvf ${TAR_FILE} oclint-release && cd -

echo "\033[32m上传文件${TAR_FILE}\033[0m" 

curl -F "path=/oclint" -F "file=@build/${TAR_FILE}" http://hulk.yupaopao.com:8004/ftp/upload/

if [[ $? -ne 0 ]]; then
	echo "\033[31m上传失败,请手动上传 curl -F \"path=/oclint\" -F \"file=@build/${TAR_FILE}\" http://hulk.yupaopao.com:8004/ftp/upload/\033[0m"
else
	echo "\033[32m上传成功\033[0m" 
fi

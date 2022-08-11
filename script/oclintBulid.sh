#! /bin/bash

projectPath=.
modulesPath=./Pods

echo $projectPath
date
echo "静态检查开始"
COLOR_ERR="\033[1;31m"    #出错提示
COLOR_SUCC="\033[0;32m"  #成功提示
COLOR_QS="\033[1;37m"  #问题颜色
COLOR_AW="\033[0;37m"  #答案提示
COLOR_END="\033[1;34m"     #颜色结束符

# 寻找项目的 ProjectName
function searchProjectName () {
  # maxdepth 查找文件夹的深度
  find $projectPath -maxdepth 1 -name "*.xcodeproj"
}

function searchWorkspaceName () {
  # maxdepth 查找文件夹的深度
  find $projectPath -maxdepth 1 -name "*.xcworkspace"
}

# modules=('BXMessageKit')

modules=('ENTPersonalRoom' 'BXOrderBusiness' 'BXGodBusiness' 'BXContentBusiness' 'BXTopicBusiness' 'BXExploration' 'BXHomeBusiness' 'BXMessageKit' 'BXCustomMessageKit' 'BXSocialBusiness' 'BXUserInfo' 'BXAboutMe' 'BXSetting' 'BXWalletBusiness' 'BXHybrid' 'BXLoginBusiness' 'BXCommonBusiness' 'BXAppLaunch' 'Lux')

function oclintForProject () {
	
    # 预先检测所需的安装包是否存在
    if which xcodebuild 2>/dev/null; then
        echo 'xcodebuild exist'
    else
        echo '🤔️ 连 xcodebuild 都没有安装，玩鸡毛啊？ 🤔️'
    fi

    if which oclint 2>/dev/null; then
        echo 'oclint exist'
    else
        echo '😠 完蛋了你，玩 oclint 却不安装吗，你要闹哪样 😠'
        echo '😠 乖乖按照博文：https://segmentfault.com/a/1190000019295332?utm_source=tag-newest 安装所需环境 😠'
    fi
    if which xcpretty 2>/dev/null; then
        echo 'xcpretty exist'
    else
        gem install xcpretty
    fi


    # 指定编码
    export LANG="zh_CN.UTF-8"
    export LC_COLLATE="zh_CN.UTF-8"
    export LC_CTYPE="zh_CN.UTF-8"
    export LC_MESSAGES="zh_CN.UTF-8"
    export LC_MONETARY="zh_CN.UTF-8"
    export LC_NUMERIC="zh_CN.UTF-8"
    export LC_TIME="zh_CN.UTF-8"
    export xcpretty=/usr/local/bin/xcpretty # xcpretty 的安装位置可以在终端用 which xcpretty找到

    path=$(searchProjectName)
    work=$(searchWorkspaceName)

    echo "project:$path"
    echo "workspace:$work"

    # myscheme=${path#*/}  # scheme名字
    # myscheme=${myscheme%\n*}
    path=${path//.\//}  # ./BridgeLabiPhone.xcodeproj -> BridgeLabiPhone.xcodeproj
    path=${path//.xcodeproj/} # BridgeLabiPhone.xcodeproj -> BridgeLabiPhone
    path=${path##*/}  # scheme名字
    work=${work//.\//}  # ./BridgeLabiPhone.xcodeproj -> BridgeLabiPhone.xcodeproj

    echo $COLOR_SUCC"scheme:$path"$COLOR_END
    echo $COLOR_SUCC"workspace:$work"$COLOR_END
	
    # 清除上次编译数据
    rm -rf ~/Library/Developer/Xcode/DerivedData/
    # if [ -d ./derivedData ]; then
    #     echo -e $COLOR_SUCC'-----清除上次编译数据derivedData-----'$COLOR_END
    #     rm -rf ./derivedData
    # fi

    # xcodebuild clean
    xcodebuild -scheme $path -workspace $work clean 
    echo $COLOR_SUCC'-----Xcode clean Success-----'$COLOR_END

    # 生成编译数据
    xcodebuild -scheme $path -workspace $work -arch arm64 -configuration Debug ONLY_ACTIVE_ARCH=YES COMPILER_INDEX_STORE_ENABLE=NO OTHER_CFLAGS="-Wno-incomplete-umbrella" | xcpretty -r json-compilation-database -o compile_commands.json

    if [ -f ./compile_commands.json ]; then
        echo $COLOR_SUCC'-----编译数据生成完毕😄😄😄-----'$COLOR_END
    else
        echo $COLOR_ERR'编译数据生成失败😭😭😭'$COLOR_END
        return -1
    fi

     
    for module in ${modules[*]}
    do
        m=$modulesPath/$module/
        echo $m
        # 生成报表 配置OCLint 规则
        #rulePath="/Users/ypp/.jenkins/workspace/ChainiOSBXCheck/oclint-master/build/oclint-release/lib/oclint/customRules"
        echo "`date "+%Y-%m-%d %H:%M:%S"` | [logadm -on@`date "+%Y%m%d%H%M%S"`] "
        
        oclint-json-compilation-database -i $m -- -report-type html \
        -rc LONG_LINE=300 \
        -rc LONG_METHOD=150 \
        -rc CYCLOMATIC_COMPLEXITY=60 \
        -rc TOO_MANY_METHODS=100 \
        -rc TOO_MANY_PARAMETERS=6 \
        -disable-rule NcssMethodCount \
        -disable-rule HighNPathComplexity \
        -disable-rule ShortVariableName \
        -disable-rule LongVariableName \
        -disable-rule HighNcssMethod \
        -disable-rule UnusedMethodParameter \
        -disable-rule ObjCAssignIvarOutsideAccessors \
        -disable-rule AssignIvarOutsideAccessors \
        -disable-rule IvarAssignmentOutsideAccessorsOrInit \
        -disable-rule UselessParentheses \
        -disable-rule WindowVisibleRule \
        -max-priority-1=100000 \
        -max-priority-2=100000 \
        -max-priority-3=100000 >> $module.html
        if [ -f $module.html ]; then
            echo $COLOR_SUCC'😄分析完毕😄'$COLOR_SUCC
        else 
            echo $COLOR_ERR'😢分析失败😢'$COLOR_ERR
        fi
        
        echo "`date "+%Y-%m-%d %H:%M:%S"` | [logadm -on@`date "+%Y%m%d%H%M%S"`] "
    done

}

 oclintForProject





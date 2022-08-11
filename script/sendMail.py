# -*- coding: utf-8 -*-
from bs4 import BeautifulSoup
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.header import Header
import os
import json
import requests
import sys

#path = sys.argv[1] if len(sys.argv) > 1 else "./"
path = "./"
version =  sys.argv[1] if len(sys.argv) > 1 else ""
print(version)
print(path)
info_array = [
    {
        'module': 'BXOrderBusiness',
        'owner': '李军灼',
        'phone': '18521067837',
        'email': [
            "lijunzhuo@bixin.cn",
            "lipengfei2@bixin.cn",
            "yangxianghui@bixin.cn",
            "sumingyang@bixin.cn",
            "zhangjun@bixin.cn",
            "zhaomizhu@bixin.cn",
        ]
    },
    {
        'module': 'ENTPersonalRoom',
        'owner': '李鹏飞',
        'phone': '18217785937',
        'email': [
            "lipengfei2@bixin.cn",
            "lijunzhuo@bixin.cn",
            "sumingyang@bixin.cn",
            "yangxianghui@bixin.cn",
            "zhangjun@bixin.cn",
            "zhaomizhu@bixin.cn",
        ]
    },
    {
        'module': 'BXHybrid',
        'owner': '李军灼',
        'phone': '18521067837',
        'email': [
            "lijunzhuo@bixin.cn",
        ]
    },
    {
        'module': 'BXGodBusiness',
        'owner': '杨向辉',
        'phone': '18317893376',
        'email': [
            "yangxianghui@bixin.cn",
        ]
    },
    {
        'module': 'BXLoginBusiness',
        'owner': '梁雪成',
        'phone': '17615192438',
        'email': [
            "liangxuecheng@bixin.cn",
        ]
    },
    {
        'module': 'BXAppLaunch',
        'owner': '梁雪成',
        'phone': '17615192438',
        'email': [
            "liangxuecheng@bixin.cn",
        ]
    },
    {
        'module': 'BXContentBusiness',
        'owner': '李飞',
        'phone': '15216891121',
        'email': [
            "lifei@bixin.cn",
        ]
    },
    {
        'module': 'BXTopicBusiness',
        'owner': '徐勉俊',
        'phone': '18217375738',
        'email': [
            "xumianjun@bixin.cn",
        ]
    },
    {
        'module': 'BXExploration',
        'owner': '徐勉俊',
        'phone': '18217375738',
        'email': [
            "xumianjun@bixin.cn",
        ]
    },
    {
        'module': 'BXHomeBusiness',
        'owner': '邬维',
        'phone': '18621561430',
        'email': [
            "wuwei01@bixin.cn",
        ]
    },
    {
        'module': 'BXMessageKit',
        'owner': '张益珲',
        'phone': '15137348047',
        'email': [
            "zhangyihui@bixin.cn",
            "qianwei@bixin.cn",
            "guowenke@bixin.cn",
        ]
    },
    {
        'module': 'BXCustomMessageKit',
        'owner': '张益珲',
        'phone': '15137348047',
        'email': [
            "zhangyihui@bixin.cn",
            "qianwei@bixin.cn",
        ]
    },
    {
        'module': 'BXSocialBusiness',
        'owner': '费俊杰',
        'phone': '18756921531',
        'email': [
            "feijunjie@bixin.cn",
            "wuwei01@bixin.cn",
        ]
    },
    {
        'module': 'BXUserInfo',
        'owner': '邬维',
        'phone': '18621561430',
        'email': [
            "wuwei01@bixin.cn",
        ]
    },
    {
        'module': 'BXAboutMe',
        'owner': '李军灼',
        'phone': '18521067837',
        'email': [
            "lijunzhuo@bixin.cn",
            "lipengfei2@bixin.cn",
            "guowenke@bixin.cn",
            "yangxianghui@bixin.cn",
        ]
    },
    {
        'module': 'BXSetting',
        'owner': '费俊杰',
        'phone': '18756921531',
        'email': [
            "feijunjie@bixin.cn",
            "wuwei01@bixin.cn",
        ]
    },
    {
        'module': 'BXWalletBusiness',
        'owner': '邬维',
        'phone': '18621561430',
        'email': [
            "wuwei01@bixin.cn",
        ]
    },
    {
        'module': 'BXCommonBusiness',
        'owner': '钱卫',
        'phone': '17521053323',
        'email': [
            "qianwei@bixin.cn",
        ]
    },
    {
        'module': 'Lux',
        'owner': '邬维',
        'phone': '18621561430',
        'email': [
            "wuwei01@bixin.cn",
            "guowenke@bixin.cn",
        ]
    }
]

modules_report_path = []
modules_bugs = []


def search():
    global modules_report_path
    for module in info_array:
        if os.path.exists(path + module["module"] + ".html"):
            print(path + module["module"] + ".html")
            modules_report_path.append(path + module["module"] + ".html")
        else:
            modules_report_path.append("")
            print("error:没有找到OCLint报告:" + path + module["module"] + ".html")


def send_mail(index):
    # 发信方邮箱，邮箱授权码 ZamGEi4DA78hNemk
    sender = 'lipengfei2@bixin.cn'
    password = '3tB39a9fcgpYLzBU'

    # 抄送方邮箱
    cc_receiver = ['zhoupengfei@bixin.cn',"zhaohuazhao@bixin.cn"]

    # 收信方邮箱
    to_receiver = info_array[index]["email"]
    print(to_receiver)
    # 服务器
    smtp_server = 'smtp.exmail.qq.com'
    server = smtplib.SMTP_SSL(host=smtp_server, port=465)
    server.login(sender, password)
    i = 0
    for e in to_receiver:
        i += 1
        cc_receiver = cc_receiver if i == 1 else []
        msg = MIMEMultipart()
        str='Dear %s,%s大礼包来了，请查收!' % (  info_array[index]["owner"] if  i == 1 else  "", info_array[index]["module"])
        msg.attach(MIMEText(str, 'plain', 'utf-8'))
        path = modules_report_path[index]
        html_file = open(path, 'rb')
        html_read = html_file.read()
        att1 = MIMEText(html_read, 'base64', 'utf-8')
        att1["Content-Type"] = 'application/octet-stream'
        att1["Content-Disposition"] = 'attachment; filename="report.html"'
        msg.attach(att1)
        # 邮件头信息
        msg['From'] = Header(sender)
        msg['To'] = Header(e)
        msg['Cc'] = ",".join(cc_receiver)
        msg['Subject'] = Header('你的快递来了，点击解锁更多哦', 'utf-8')
        server.sendmail(sender, [e] + cc_receiver, msg.as_string())
    server.quit()


def find_bug_num():
    global modules_bugs
    for module in modules_report_path:
        if module == "":
            modules_bugs.append(0)
            continue
        bugs1 = 0
        bugs2 = 0
        bugs3 = 0
        ignore_Count = 0
        module_name = module[2:-5]
        print("module_name:" + module_name)

        html_file = open(module, 'rb')
        html_read = html_file.read()
        soup = BeautifulSoup(html_read, 'lxml')
        tr_arr = soup.findAll(name="td", attrs={"class": "priority1"})
        tr_arr2 = soup.findAll(name="td", attrs={"class": "priority2"})
        # tr_arr3 = soup.findAll(name="td", attrs={"class": "priority3"})
        tr_arr3 = soup.findAll(name="td", attrs={"class": "cmplr-warning"})
        tr_arr4 = soup.findAll(name="td")
        tr_arr5 = soup.findAll(name="tr")
        #过滤warijg
        for tr in tr_arr5:
            if tr.get_text().find("compiler warning") >= 0:
                if tr.get_text().find("'APP_STORE' macro redefined") >= 0 and tr.get_text().find(module_name) >= 0:
                    ignore_Count += 1
                if tr.get_text().find(module_name) < 0:
                    ignore_Count += 1
        #替换path
        for tr in tr_arr4:
            pyth = "/Users/ypp/.jenkins/workspace/ChainiOSBXCheck/"
            if tr.get_text().find(pyth) >= 0:
                pythStr = tr.get_text().strip()[len(pyth):]
                tr.string.replace_with(pythStr)

        html_file1 = open(module, 'w')
        html_file1.write(soup.prettify())

        print("ignore_Count:" + str(ignore_Count))

        for tr in tr_arr:
            bugs1 = bugs1 + int(tr.get_text())
            break
        for tr in tr_arr2:
            bugs2 = bugs2 + int(tr.get_text())
            break
        for tr in tr_arr3:
            bugs3 = bugs3 + int(tr.get_text()) - ignore_Count
            break
        

        # bugs1_str = "严重问题%s个," % bugs1 if bugs1 > 0 else ""
        # bugs2_str = "中等问题%s个," % bugs2 if bugs2 > 0 else ""
        # bugs3_str = "可优化问题%s个" % bugs3 if bugs3 > 0 else ""
        
        bugs1_str = "严重问题%s个," % bugs1 if bugs1 > 0 else ""
        bugs3_str = "编译警告%s个" % bugs3 if bugs3 > 0 else ""
        bugs2_title = "中等问题%s个"
        
        if bugs3 > 0:
           bugs2_title = "中等问题%s个,"
           
        bugs2_str = bugs2_title % bugs2 if bugs2 > 0 else ""

        if len(bugs1_str + bugs2_str + bugs3_str) == 0:
            modules_bugs.append(0)
        else:
            modules_bugs.append(bugs1_str  + bugs2_str + bugs3_str)

def send_msg(content, mobile):
    headers = {'Content-Type': 'application/json;charset=utf-8','Connection': 'close'}
    data = {
        "msgtype": "text",
        "text": {
            "content": content
        },
        "at": {
            "atMobiles":
                mobile
            ,
            "isAtAll": False  # 此处为是否@所有人
        }
    }
    s = requests.session()
    #"https://oapi.dingtalk.com/robot/send?access_token=df1c1557c5e289edefce5ade54c70b39ee8f44ea9c572bdc4ee4bfd51c4cd270"
    #test='https://oapi.dingtalk.com/robot/send?access_token=3295997ce7581bcbecd84dbb6039d952026bb4c8cd0eecaa5b6d4b3e97ca0615'
    s.keep_alive = False
    r = requests.post('https://oapi.dingtalk.com/robot/send?access_token=df1c1557c5e289edefce5ade54c70b39ee8f44ea9c572bdc4ee4bfd51c4cd270',
                      data=json.dumps(data), headers=headers)
    return r.text


if __name__ == '__main__':
    search()
    find_bug_num()
    index = 0
    tipStr = ""
    phone = []
    print(modules_bugs)
    for obj in info_array:
        if modules_bugs[index] != 0:
            tipStr = tipStr + '@%s，%s模块共查询到%s。\n' % (obj["owner"], obj["module"], modules_bugs[index])
            send_mail(index)
            phone.append(obj["phone"])
        index += 1
    if tipStr != "":
       tipStr = "代码管家提醒你，以下问题基于%s分支检查，请移步邮箱查看:\n" % (version) + tipStr
       send_msg(tipStr, phone)

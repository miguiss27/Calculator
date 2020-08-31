#import libraries

import math
#import matplotlib
import numpy
#import sympy
import subprocess as sub
#import sys
import time
import keyboard

#define variables

res=""
exe=""
cmd=""
timsl=0.25
lcmd=""
#define redirections

cmred={"quit":"m0","basic":"m1","math":"m2","numpy":"m3","emulationstation":"m0","(":"m0",")":"m0","execute":"m0"}
redba={"m1":"m0","m2":"m0","m3":"m0","m4":"m1"}
rednx={"m1":"m4"}

#define menu

mn="m0"

m0="//                  /                  /                  /                  //\n//      quit        /      math        /    matplotlib    /      numpy       //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//      sympy       /     pyquim       /       pyfis      /      basic       //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//       none       /       none       /       none       /       none       //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//     execute      /      delete      /      startx      /     retropie     //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////"
m1="//                  /                  /                  /                  //\n//      back        /        1         /        2         /        3         //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//        +         /        4         /        5         /        6         //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//        -         /        7         /        8         /        9         //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//       next       /        /         /        0         /        *         //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////"
m2="//                  /                  /                  /                  //\n//      back        /       sqrt       /       pow        /       log        //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//       cos        /      acos        /       fabs       /    factorial     //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//       sin        /      asin        /        pi        /        e         //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//       tan        /      atan        /       none       /       none       //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////"
m3="//                  /                  /                  /                  //\n//      back        /      expand      /     factors      /     symbols      //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//      limit       /      solve       /       subs       /     symplify     //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//      evalf       /   init_session   /       sqrt       /       none       //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//       cos        /       sin        /        tg        /       none       //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////"
m4="//                  /                  /                  /                  //\n//      back        /        (         /        )         /        ,         //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//        //        /        %         /        **        /        ;         //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//       none       /       none       /       none       /       none       //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////\n//                  /                  /                  /                  //\n//       none       /       none       /       none       /       none       //\n//                  /                  /                  /                  //\n///////////////////////////////////////////////////////////////////////////////"

#define key menu

d0={"1":"quit","2":"math","3":"","4":"numpy","q":"","w":"","e":"","r":"basic","a":"","s":"","d":"","f":"","z":"execute","x":"delete","c":"startx","v":"emulationstation"}
d1={"1":"back","2":"1","3":"2","4":"3","q":"+","w":"4","e":"5","r":"6","a":"-","s":"7","d":"8","f":"9","z":"next","x":"/","c":"0","v":"*"}
d2={"1":"back","2":".sqrt(","3":".pow(","4":".log(","q":".cos(","w":".acos(","e":".fabs(","r":".factorial(","a":".sin(","s":".asin(","d":".pi","f":".e","z":".tan(","x":".atan(","c":"","v":""}
d3={"1":"back","2":"","3":".factors(","4":".symbols(","q":".limit(","w":".solve(","e":".subs(","r":".symplify(","a":".evalf(","s":".init_sesion(","d":".sqrt(","f":"","z":".cos(","x":".sin(","c":".tg(","v":""}
d4={"1":"back","2":"(","3":")","4":",","q":"//","w":"%","e":"**","r":";","a":"","s":"","d":"","f":"","z":"","x":"","c":"","v":""}

#menu dictionary

dmn={"m0":m0,"m1":m1,"m2":m2,"m3":m3,"m4":m4}


#menu print

def showmenu(menu, comand, result):



    lngcmd=len(comand)
    lngres=len(str(result))

    
    if lngcmd<35 and lngcmd%2!=0 or lngcmd==32:
        comand=str((int((35-lngcmd)/2)-1)*" ")+comand+str((int((35-lngcmd)/2)-1)*" ")
        if lngcmd==32:
            comand+=" "            
    elif lngcmd<35 and lngcmd%2==0 and lngcmd!=34:
        comand=str((int((36-lngcmd)/2)-2)*" ")+comand+str((int((35-lngcmd)/2))*" ")

    if lngcmd>35 or lngcmd==33 or lngcmd==34 or lngcmd==35:
        comand=comand[(lngcmd-33):]

   
    if lngres<35 and lngres%2!=0 or lngres==32:
        result=str((int((35-lngres)/2)-1)*" ")+str(result)+str((int((35-lngres)/2)-1)*" ")
        if lngres==32:
            result+=" "
    elif lngres<35 and lngres%2==0 and lngres!=34:
        result=str((int((35-lngres)/2)-1)*" ")+str(result)+str(int((35-lngres)/2)*" ")

    if lngres>35 or lngcmd==33 or lngcmd==34 or lngcmd==35:
        result=str(result)[(lngres-33):]
    
    print("///////////////////////////////////////////////////////////////////////////////")    
    print("///////////////////////////////////////////////////////////////////////////////")
    print("///////////////////////////////////////////////////////////////////////////////")
    print("///////////////////////////////////////////////////////////////////////////////")
    print("//                                     /                                     //")
    print("// ",comand," / ",result," //")
    print("//                                     /                                     //")
    print("///////////////////////////////////////////////////////////////////////////////")

    print(dmn[menu])
    
    print("///////////////////////////////////////////////////////////////////////////////")
    print("///////////////////////////////////////////////////////////////////////////////")
    print("///////////////////////////////////////////////////////////////////////////////")
    print("///////////////////////////////////////////////////////////////////////////////")

#helper for presses

def press(string):
    for key in string:
        keyboard.press(key)
        time.sleep(0.001)
        keyboard.release(key)

#main execution


qut=False
while qut!=True:


    showmenu(mn,cmd,res)
    res=""
    key=keyboard.read_key()
#    key=input()
#    key=str(sys.stdin.read(1))
    sub.call("clear")
    time.sleep(timsl)


    lng=len(cmd)
    
    try:
        if mn=="m0":
            cmd+=d0[key]
            if d0[key]!="delete":
                lcmd+=str(len(d0[key]))
        elif mn=="m1":
            cmd+=d1[key]
            lcmd+=str(len(d1[key]))
        elif mn=="m2":
            cmd+=d2[key]
            lcmd+=str(len(d2[key]))
        elif mn=="m3":
            cmd+=d3[key]
            lcmd+=str(len(d3[key]))
        elif mn=="m4":
            cmd+=d4[key]
            lcmd+=str(len(d4[key]))
    except:
        mn="m0"

    lcmd=lcmd.replace("0","")

    try:
        if mn=="m0":
            fnd=cmd[lng:]
            mn=cmred[fnd]
    except:
        mn="m0"
    
    if (cmd.find("emulationstation")>=0):
        sub.call(["su", "pi", "-c", "emulationstation"])
        sub.call("clear")
    if (cmd.find("startx")>=0):
        sub.call("lightdm")
        keyboard.send('ctrl+alt+F1')
        sub.call("clear")
    if (cmd.find("back")>=0):
        mn=redba[mn]
    if (cmd.find("next")>=0):
        mn=rednx[mn]
    if (cmd.find("execute")>=0):
        exe="res="+cmd[:lng]
    if (cmd.find("quit")>=0):
        qut=True
    if (cmd.find("delete")>=0):
        try:
            cmd=cmd.replace("delete","")
            cmd=cmd[:-(int(lcmd[-2]))]
            lcmd=lcmd[:-1]
        except:
            cmd=cmd.replace("delete","")

        
    cmd=cmd.replace("basic","")      
    cmd=cmd.replace("back","")
    cmd=cmd.replace("emulationstation","")
    cmd=cmd.replace("execute","")
    cmd=cmd.replace("next","")
    cmd=cmd.replace("startx","")

    try:
        exec(exe,globals(),locals())
        exe=""
    except:
        cmd=""
        exe=""

    
#exiting routine

sub.call("clear")
quit()

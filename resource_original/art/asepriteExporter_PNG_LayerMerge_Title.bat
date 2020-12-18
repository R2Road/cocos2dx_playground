cls

echo off

echo arg1 - %1
echo arg2 - %2
echo arg3 - %3

set scale_value=%3
if [%3]==[] set scale_value=1

echo scale_value - %scale_value%


echo on

"c:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"^
 -b^
 %1^
 --scale %scale_value%^
 --filename-format "{title}.{extension}"^
 --save-as %2

pause
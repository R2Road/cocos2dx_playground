cls
echo arg1 - %1
echo arg2 - %2
echo arg3 - %3

"%AsepriteRoot%\Aseprite.exe"^
 -b^
 %1^
 --scale %3^
 --filename-format "{title}_{tag}_{tagframe}.{extension}"^
 --save-as %2

pause
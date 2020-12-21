cls

set aseprite_file_name=%~n0.aseprite
set png_file_name=%~n0.png
set scale_value=3

call ../asepriteExporter_PNG_LayerMerge_Title_Tag_TagFrame_Scale.bat^
 %aseprite_file_name%^
 %png_file_name%^
 %scale_value%
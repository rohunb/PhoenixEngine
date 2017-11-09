./premake5 --file=../Solution/Premake.lua xcode4

rm -rf ../Solution/Game/Game.xcodeproj/xcuserdata/*.xcuserdatad/xcschemes
rm -rf ../Solution/PhoenixEngine/PhoenixEngine.xcodeproj/xcuserdata/*.xcuserdatad/xcschemes

echo "
 require 'xcodeproj'
 project = Xcodeproj::Project.open '../Solution/PhoenixEngine/PhoenixEngine.xcodeproj'
 project.recreate_user_schemes
 project.save
 project = Xcodeproj::Project.open '../Solution/Game/Game.xcodeproj'
 project.recreate_user_schemes
 project.save
" | ruby
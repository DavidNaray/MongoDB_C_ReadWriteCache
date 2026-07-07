MongoDB c driver got compiled and its in the c root folder: C:\mongo-c-driver\install

compile this to avoid gcc headers conflicting with the msvc
& "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"


compile it like: cl test.c /I C:\mongo-c-driver\install\include\mongoc-2.4.0 /I C:\mongo-c-driver\install\include\bson-2.4.0 /link /LIBPATH:C:\mongo-c-driver\install\lib mongoc2.dll.lib bson2.dll.lib

run it all by going into cmd and typing test.exe



vcpkg had to be installed which makes pthread compatible with the msvc (c/c++ desktop developer mircrosoft developer visual studio environment) 

git clone https://github.com/microsoft/vcpkg

cd vcpkg

.\bootstrap-vcpkg.bat    <-this runs the commands that do the business

.\vcpkg integrate install

.\vcpkg install pthreads:x64-windows    <-actually gets pthreads


then finally in c_cpp_properties.json you include the include of the vcpkg where pthreads.h lies


compile it like: gcc Main.c scheduler.c config.c C:/Users/david/Documents/CodingProjects/C_MMO_RPG_rewrite/CJSON_lib/cjson/cJSON.c -I"C:/Users/david/Documents/CodingProjects/C_MMO_RPG_rewrite/CJSON_lib/cjson" -o DistributedNodes -lpthread
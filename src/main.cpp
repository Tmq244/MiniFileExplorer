#include<iostream>
#include<filesystem>

#include"App.h"
#include"Commands.h"
#include"FileSystemContext.h"

using namespace std;

int main(int argc,char* argv[]){
    namespace fs=filesystem;

    FileSystemContext ctx;

    try{
        if(argc>1){
            ctx.currentDir=fs::weakly_canonical(argv[1]);
        }else{
            ctx.currentDir=fs::current_path();
        }
    }catch (const exception& ex){
        cerr<<"Error determining initial directory: "<<ex.what()<<endl;
        return 1;
    }

    ctx.homeDir=getenv("HOME");

    CommandRegistry registry;
    registerBuiltInCommands(registry);

    cout<<"Welcome to MiniFileExplorer!\n";

    App app(ctx,registry);
    app.run();

    return 0;
    
}

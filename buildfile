
project := "FudgeProject".

project_version := "1.0".

buildfile_version := "1.0".

url_to_src := "https://github.com/JHG777000/Fudge/archive/master.zip".

build FudgeBuild.

 options.

  on test_enable("-t", "--test", "Enable Fudge test.").

 end options.

 get test_enable.

 message("Building RKLib...\n").

 url URLForRKLib("https://raw.githubusercontent.com/JHG777000/RKLib/master/buildfile").

 subproject RKLibProject(URLForRKLib,nil).

 message("Building Fudge...\n").

 make filepath include_path from "resources" to "include".

 make filepath rklib_include_path from "resources" to "include" from RKLibProject.

 files FudgeFiles("src.directory").

 sources FudgeSource(FudgeFiles).

 compiler FudgeCompilerFlags("-Wall", "-I " + include_path, "-I " + rklib_include_path).

 toolchain FudgeToolChain("clang",FudgeCompilerFlags).

 output Fudge("library",FudgeSource,FudgeToolChain).

 if ( test_enable ).

  grab RKLib from RKLibProject.

  files FudgeTestFiles("main.c","Example1.directory","Example2.directory").

  sources FudgeTestSource(FudgeTestFiles,Fudge,RKLib).

  output FudgeTest("application",FudgeTestSource,FudgeToolChain).

  launch(FudgeTest).

 end if.

end build.

build clean_build.

 url URLForRKLib("https://raw.githubusercontent.com/JHG777000/RKLib/master/buildfile").

 subproject RKLibProject(URLForRKLib,"-b clean_build").

 message("Cleaning Fudge...\n").

 clean("build").

end build.

default FudgeBuild.




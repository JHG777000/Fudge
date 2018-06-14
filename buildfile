
project := "FudgeProject".

project_version := "1.1.2".

buildfile_version := "1.0".

url_to_src := "https://github.com/JHG777000/Fudge/archive/master.zip".

build FudgeBuild.

 options.

  on test_enable("-t", "--test", "Enable Fudge test.").

  on toolchain_select("-s", "--select_toolchain=tool", "Select toolchain, clang or gcc.").

 end options.

 get test_enable.

 get toolchain_select.

 if ( toolchain_select != "clang" && toolchain_select != "gcc" ).

  var toolchain_select := "gcc".

 end if.

 message("Building RKLib...").

 url URLForRKLib("https://raw.githubusercontent.com/JHG777000/RKLib/master/buildfile").

 subproject RKLibProject("local",URLForRKLib,nil).

 grab RKLib from RKLibProject.

 return_output RKLib.

 message("Building Fudge...").

 make filepath include_path from "resources" to "include".

 make filepath rklib_include_path from "resources" to "include" from RKLibProject.

 files FudgeFiles("src.directory").

 sources FudgeSource(FudgeFiles).

 compiler FudgeCompilerFlags("-Wall", "-I " + include_path, "-I " + rklib_include_path).

 toolchain FudgeToolChain(toolchain_select,FudgeCompilerFlags).

 output Fudge("library",FudgeSource,FudgeToolChain).

 if ( test_enable ).

  message("Running FudgeTest...").

  files FudgeTestFiles("main.c","Example1.directory","Example2.directory").

  sources FudgeTestSource(FudgeTestFiles,Fudge,RKLib).

  output FudgeTest("application",FudgeTestSource,FudgeToolChain).

  launch(FudgeTest).

  message("Ran FudgeTest.").

 end if.

end build.

build clean_build.

 url URLForRKLib("https://raw.githubusercontent.com/JHG777000/RKLib/master/buildfile").

 subproject RKLibProject("local",URLForRKLib,"-b clean_build").

 message("Cleaning Fudge...").

 clean("build").

end build.

default FudgeBuild.




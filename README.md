<div align="center">

# Vim Debug Adapter Protocol Tutorial

[Introduction](#Introduction) •
[Outcome](#Outcome) •
[Provisions](#Provisions) •
[Setup](#Setup) •
[Configurations](#Configurations) •
[Keybindings](#Keybindings) •
[Conclusion](#Conclusion)

</div>

# Introduction

With the advent of vim8+ providing async capabilities, we have gotten features such as asynchronous linting (e.g. [ALE](https://github.com/dense-analysis/ale)), asynchronous Language Server Protocols, a.k.a. "LSP" (e.g. [vim-lsp](https://github.com/prabirshrestha/vim-lsp), [Microsoft Language Server Protocol](https://microsoft.github.io/language-server-protocol/)), and even asynchronous autocompletion [asyncompletion](https://github.com/prabirshrestha/asyncomplete.vim)). This tutorial is focused on succinctly introducing the Debug Adapter Protocol, a.k.a. "DAP" (i.e. [Microsoft Debug Adapter Protocol](https://microsoft.github.io/debug-adapter-protocol/)). When taken together, these tools provide excellent productivity boosts when developing and maintaining software on top of that which comes from ever increasing mastery of `vim` itself.

Just as the Language Server Protocol provides symbol navigation interface for inspecting code and rapidly navigating a codebase, the Debug Adapter Protocol is an interface layer to any debugger implementing the protocol. The `vim` plugin [vimspector](https://github.com/puremourning/vimspector) implements the DAP protocol for `vim` and provides convenience keybindings for many useful activities while using a debugger. The result is a visual debugging workspace for any language one has configured. 

The pipeline from (A) to (E) is:

- (A) `vim`
- (B) `vimspector` plugin (the DAP layer)
- (C) language-specific adapter (e.g. `CodeLLDB`, `vscode-cpptools`)
- (D) language-specific debugger (e.g. `lldb`, `gdb`)
- (E) use binary -or- attach to remote server

# Outcome

By the end of this tutorial, one will have:

- preliminary understanding of how vimspector works
- basic vimspector setup for `lldb` and `C` programming
- a convenience keybinding for making `lldb` calls while debugging
- keybindings for performing common debugging tasks (such as creating breakpoints, launching the debugger, and saving/loading breakpoints to file)

# Provisions

The following is provided:

- `HOME` - a directory that corresponds to your system home directory. The excerpt `vimrc` is here.
- `SAMPLE_PROJECT` - a sample C project with its `.vimspector.json` config.
- `VIMSPECTOR_BASEDIR` - a directory that corresponds to your vimspector base directory (see below).
 
# Setup

While there are many possible configurations possible even for one compiler, we will focus on the fastest path to getting a working, general implementation. From there, one can read the docs and tune to one's own needs and desires. 

## Set the base directory

In the`vimrc` set the base directory to a convenient spot such as:

```
let g:vimspector_base_dir=expand( '$HOME/.vim/vimspector-config' )
```

Otherwise, you will have to add certain config files deep in the directory hierarchy of the plugin itself. 

**NOTE:** This directory will be referred to as `$VIMSPECTOR_BASEDIR` in this tutorial.

## Gadgets

Gadgets correspond to adapters, the things that the `vimspector` plugin is acting as a bridge between. Practically speaking, the gadgets and adapters are one-to-one, though technically there could be a many-to-one relationship between them. 

If one attempts to launch a debugger without the configured gadget/adapter, a prompt will pop up to install it. It is possible to add configuration for adapter installs or even to run a command in `vim` such as `:VimspectorInstall <adapter>` where `<adapter>` could be something like `debugpy` for `Python` or `CodeLLDB` for `C` projects. For our purposes it will be sufficient to have it prompt us, installing upon first use. More info is found on the project page. 

The installed gadgets will go in `$VIMSPECTOR_BASEDIR/gadgets/$OS/$GADGETNAME` where `$OS` is the operating system (e.g. "macos", "linux") and `$GADGETNAME` is one of the installed adapters such as `CODELLDB` or `vscode-cpptools`.

## .vimspector.json

The most important file is the `.vimspector.json`. This configures the adapter as well as parameters such as how the debugger is launched/attached, port configuration for remote debugging, and command line arguments, and many more. This tutorial is meant to show a simple workflow to get one started. Afterwards, one should reference the docs as project needs demand. 

The `.vimspector.json` file lives in each project root. There are also other `.vimspector.json` files that one can create and place at `$VIMSPECTOR_BASEDIR/configurations/$OS/$LANGUAGE/$LANGUAGE.json`. The workflow we will use here is to create a `$LANGUAGE.json` in the aforementioned directory. That will serve as the "default" settings. Then we will modify certain project-specific fields in the `.vimspector.json` to suit our individual project's needs. For instance, the default might have the build executable be in the root of the code repo whereas the project one is working on might have a different directory structure. 

First, let us explore the structure of the `.vimspector.json`. Notice that these `JSON` files have two top level fields: `adapters` and `configurations`. 

From the [Configuration Documentation](https://puremourning.github.io/vimspector/configuration.html) we have:

```
{
  "adapters": { <object mapping name to <adapter configuration> },
  "configurations": { <object mapping name to <debug configuration> }
}
```

# Configurations

## Adapter configurations

A useful reason to create an adapter configuration is for remote-debugging purposes. For instance, the following snippet (again from the Configuration reference) sets up a `gdbserver` for remote debugging:

```
{
  "adapters": {
    "cpptools-remote": {
      "command": [
        "${gadgetDir}/vscode-cpptools/debugAdapters/bin/OpenDebugAD7"
      ],
      "name": "cppdbg",
      "configuration": {
        "type": "cppdbg" },
      "launch": {
        "remote": {
          "host": "${host}",
          "account": "${account}",
          // or, alternatively "container": "${ContainerID}"

          "runCommand": [
            "gdbserver",
            "--once",
            "--no-startup-with-shell",
            "--disable-randomization",
            "0.0.0.0:${port}",
            "%CMD%"
          ]
        },
        ...excerpt...
}
```

This creates an adapter configuration called `cpptools-remote` and configures it with a `command` of the relevant debugger executable, a `name`, and a `configuration`. It also sets up a `launch` variant with information needed for launching a remote debugging session. An `attach` would also be useful.

For this tutorial, we will not focus on "adapter configuration". Instead, we will see how to get up and rolling quickly with just a local "debug configuration". 

## Debugger configurations

Consider the configuration for local debugging with `lldb`. The debugger configuration part of our `.vimspector.json` (in the `configurations` section) might look like:

```
{
  "configurations": {
    "C - LLDB": {
      // Local C debugging with native-arch
      "adapter": "vscode-cpptools",
      "variables": {
        "targetArch" : {
          "shell": [
            "uname",
            "-m"
          ]
        } 
      },
      "configuration": {
        "request": "launch",
        "program": "${workspaceRoot}/${fileBasenameNoExtension}",
        "StopAtEntry": true,
        "MIMode": "lldb",
        "targetArchitecture": "${targetArch}",
        "setupCommands": [
          {
            "description": "Enable pretty-printing for lldb",
            "text": "-enable-pretty-printing",
            "ignoreFailures": true
          }
        ]
      }
    }
  }
}
```

Here we see a debugging configuration called `C - LLDB`. Notice that we have read the shell command `uname -m` into the variable `targetArch` which we then use below in the `configuration` section for the `targetArchitecture` field. Furthermore, we have set the compiler to be `lldb`. One could change this to be `gdb` if on `Linux`. In this particular case we have the `setupCommands` to enable "pretty-printing". Since `lldb` has pretty-printing on by default, this does nothing. However, if one were to change the `MIMode` to `gdb` it would activate pretty-printing on `gdb`. 

This `.vimspector.json` will actually be our default in our case (located in `$VIMSPECTOR_BASEDIR/configuration/$OS/c/c.json`) and will be referred to as `c.json`. The `$LANGUAGE.json` is a useful convention, though one can technically have multiple language settings in a given file and can therefore structure these directories with a bit of latitude. In fact, `vimspector` will populate a list of all the applicable debugger configurations it knows about when it is launched. 

*ASIDE:* Notice when you finally start using this that if you were to comment out the entire `variables` section in `c.json`, the prompt would ask you to fill in the `targetArchitecture` by hand when you try to use this debugger. You would do this each time the debugger is run. This could be useful to know. 

## Project .vimspector.json

Every project needs a `.vimspector.json` file. This file, placed in the project's root (e.g. `git` repo root directory), overwrites certain aspects of the `$VIMSPECTOR_BASEDIR` `.json` files (e.g. `c.json`) where appropriate. An example of a project specific overwrite is:

```
{
  "configurations": {
     "C - LLDB [project]": {
          "extends": "C - LLDB",
          "variables" : {
            "buildapp" : "build/app"
          },
          "configuration": {
            "program": "${workspaceRoot}/${buildapp}"
          }
      }
  }
}
```

Here we have inherited the `C - LLDB` configuration from `c.json` via the `extends` field. However, we have modified the build directory to `build/app` rather than just the project root. We have named our modification "C - LLDB [project]". Both "C - LLDB [project]" and "C - LLDB" would show up as potential debuggers to use upon launch. 

**NOTE:** There are actually ways to avoid placing a `.vimspector.json` per project, though I would not worry about that for now. Keep it simple. 

## Summary

To summarize so far, we have two sets of `.vimspector.json` files:
- a project local file called `.vimspector.json` located in the project root
- a series of `$LANGUAGE.json` (or `$PARADIGM.json`, etc.) files to be used as default configurations to be modified as needed by the project-specific `.vimspector.json`. 

Gadgets are installed automatically when you need them. You can fine-tune that behavior if you want, but it is not strictly necessary. 

We have provided a convenient `$VIMSPECTOR_BASEDIR` in the `vimrc`. 

Now we are ready to use this to debug. 

# Keybindings

Debuggers are feature-rich with near endless possibilities for how they can be configured. The goal here is to set up some keybindings and utility workflows to get one started. From there, it can be adjusted to one's taste. 

At a high level, we have defined some keybindings via the included `vimrc` excerpt for uses like launching, stepping through functions, setting breakpoints, and loading/saving breakpoint states and sessions.

## Keybinding for lldb command prompt

Not all debugger functionality has a dedicated function in the `vimspector` plugin (see [the plugin page](https://github.com/puremourning/vimspector) for what is supported). This is fine as one can create `vim` functions/keybindings to map to any functionality one might find is missing in one's personal workflow.

For starters, we have added a convenience function called `LLDBPrompt()`. By default when using the plugin, one has to be in the `Vimspector.Console` window, press `i` for insert mode and then type `-exec <my lldb command><CR>`. That's a lot of keystrokes (seven!) for such a common escape hatch command. Instead, we have wrapped all of that in this `LLDBPrompt()` function which is called from `Normal` mode in any window and is bound to a key `<localleader>z`. It shows the same prompt one would see when running `lldb` from the CLI. 

`(lldb)         `

Thus at minimum, we now have access to the same `lldb` we would have on the terminal with a GUI for everything that `vimspector` provides (i.e. the "bonus" stuff).

One could use this like so:

`(lldb) po myVar`

The outcome (value of `myVar`) will show up in the `Vimspector.Console` window. 

**NOTE:** One can get far with the commands 
- `backtrace` or `bt` to see the call stack
- `list` or `l` to list nearby lines
- `print` or `p` or `po` variants to inspect variables
- `breakpoint` to set breakpoints
- `watchpoint` to set watchpoints based on some conditions

## Keybindings for Session handling (save/restore breakpoints and vim session)

There are also some keybindings for saving/loading breakpoints and `vim` sessions. One would begin by setting some breakpoints and typing `<localleader>ss` for "session save". This saves a `.vimspector-session.vim` and a `.vimspector-sessionx.vim`. When one wants to resume the debugging, one could type `<localleader>sl` for "session load". This will restore any breakpoints, watchpoints, etc. that had been saved as well as anything that would be stored in a typical `vim` session. One could also call `vim -S vimspector-session.vim` and the breakpoint info would also automatically load. All of this is due to the bindings and helper function in the provided `vimrc`. The `vimspector` plugin does not quite work like this out of the box.

**NOTE**: `*.x.vim` files are automatically sourced when a session of the same name without the "x" is sourced. In our case, we save the typical vim session state in `.vimspector-session.vim` and the `.vimspector-sessionx.vim` with all the breakpoint info loads seamlessly along with it. The default `.vimspector.session` name has been modified via the `vimrc` excerpt to facilitate this workflow.

## Keybindings for some common tasks

Some keybindings for things such as launching a debugger, setting and editing breakpoints, stepping through functions have been added. See the included excerpt `vimrc` for those. Over time, one might have a desire to extend `lldb` functionality by calling `Python` scripts, etc. and can easily extend this philosophy by binding those things to `vim` shortcuts. What is presented here should be a sufficient starting point. 

## JSON comments

It should be noted that `vimspector` allows C-style comments in its .json files and strips them out before processing. A syntax highlighting `automod` has been added to the excerpt `vimrc` to change error coloring to comment coloring for your sanity. 

# Conclusion

Remember to compile your project (and with debug symbols)! A build script is provided. 

That aside, installation involves
    - adding this excerpt `vimrc` to your `.vimrc`
    - adding the included `c.json` to your `$VIMSPECTOR_BASEDIR/configurations/$OS/c/c.json`
    - adding the `.vimspector.json` to the project root
    - installing the necessary gadget on first use (should be prompted automatically)

Other than that, explore your code achieve some things, and maybe later see what other things you could configure with the docs. Keep it simple.

# References
- https://github.com/da-mago/Vimspector_configuration/

<div style="width:60%;height:200px;text-align:center;border:14px solid #808080;border-top:none;border-left:none;border-bottom:none;display:inline-block">
    <div style="border:4px solid #808080;border-radius:8px;width:95%;height:100%;background-color: rgb(209, 209, 209);">
        <div style="width:100%;height:30%;text-align:center;line-height:60px;font-size:26px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;">VE482 Lab Report</div>
        <div style="width:100%;height:18%;text-align:center;line-height:26px;font-size:20px;font-familny:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Lab 6</b> - Fall 2020</div>
        <div style="width:100%;height:57%;text-align:center;font-size:16px;line-height:22px;font-family: 'Courier New', Courier, monospace;font-weight:300;"><br><b>Name: Wu Qinhang<br>ID: 518370910041<br>Email: william_wu@sjtu.edu.cn<br></b></div>
    </div>
</div>
<div style="width:35%;height:200px;display:inline-block;float:right">
    <div style="width:100%;height:25%;text-align:center;line-height:55px;font-size:20px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Table of Contents</b></div>
    <div style="width:100%;height:75%;text-align:left;margin-left:2px;line-height:30px;font-size:13px;font-family:Verdana, Geneva, Tahoma, sans-serif;font-weight:300;">• plugin structure<br>• real plugin structure analysis<br>• plugin architecture design</div>
</div>




[TOC]

## Zathura Plugin

Steps to build Zathura

```shell
#!/bin/bash
# Environment: Focal
# install essential tools
sudo apt install libglib2.0-dev libgtk-3-dev libgirara-dev doxygen check libjson-c-dev gettext clang 
# clone the library of zathura locally
# Note: currently, the develop branch cannot be compiled correctly (2020-11-6)
git clone ssh://git@git.pwmt.org:10022/pwmt/zathura.git
cd zathura
git checkout master
# build
# Note: if girara not found, compile and install it by yourself from https://pwmt.org/projects/girara/installation/
cd zathura
mkdir build
meson build 
cd build
ninja 
ninja install # install Zathura onto the machine
```

### Zathura plugin implementation[^1][^2][^3]

The mechanism of plugin management of Zathura is a combination of **Discovering** and **Registering**. Zathura plugin-related files will be moved to a path specified by the variable `plugin_path` during the installation. After that, Zathura searches for and loads them when running. During Registering, the following information will be registered:

- name
- version information (major, minor, revision)
- a list of supported content types
- document functions

Zathura core defines a set of functionality that the plugin mush implement in order for the main program to run correctly:

- open
- free
- page init/clear
- page rendering

The skeleton of a simple Zathura plugin is provided as follow:

```c
#include <zathura/plugin-api.h>

ZATHURA_PLUGIN_REGISTER_WITH_FUNCTIONS(
  "txt",
  VERSION_MAJOR, VERSION_MINOR, VERSION_REV,
  ZATHURA_PLUGIN_FUNCTIONS({
    .document_open            = txt_document_open,
    .document_free            = txt_document_free,
    .page_init                = txt_page_init,
    .page_clear               = txt_page_clear,
    .page_render_cairo        = txt_page_render_cairo,
  }),
  ZATHURA_PLUGIN_MIMETYPES({
    "text/plain"
  })
)
  
/**
 * Open a txt document
 *
 * @param document Zathura document
 * @return ZATHURA_ERROR_OK if no error occurred otherwise see
 *   zathura_error_t
 */
GIRARA_HIDDEN zathura_error_t txt_document_open(zathura_document_t* document);

/**
 * Closes and frees the internal document structure
 *
 * @param document Zathura document
 * @return ZATHURA_ERROR_OK if no error occurred otherwise see
 *   zathura_error_t
 */
GIRARA_HIDDEN zathura_error_t txt_document_free(zathura_document_t* document, void* txt_document);

/**
 * Initializes the page with the needed values
 *
 * @param page The page object
 * @return ZATHURA_ERROR_OK when no error occurred, otherwise see
 *    zathura_error_t
 */
GIRARA_HIDDEN zathura_error_t txt_page_init(zathura_page_t* page);

/**
 * Frees a PostScript page
 *
 * @param page Page
 * @return ZATHURA_ERROR_OK if no error occurred otherwise see
 *   zathura_error_t
 */
GIRARA_HIDDEN zathura_error_t txt_page_clear(zathura_page_t* page, void* txt_page);

/**
 * Renders a page onto a cairo object
 *
 * @param page Page
 * @param cairo Cairo object
 * @param printing Set to true if page should be rendered for printing
 * @return ZATHURA_ERROR_OK if no error occurred otherwise see
 *   zathura_error_t
 */
GIRARA_HIDDEN zathura_error_t txt_page_render_cairo(zathura_page_t* page, void* txt_page, cairo_t* cairo, bool printing);

```

### zathura-txt implementation[^4] [^5] [^10]

See `./zathura-txt/*` for details.

step to rebuild Zathura (additional plugin added)

```shell
#!/bin/bash
# make sure the subdirectory build/ exists
meson build
cd build
ninja
ninja install
```

Before the rebuild:

![image-20201107004344572](C:\Users\William Wu\Documents\Mainframe Files\UMJI-SJTU\1 Academy\20 Fall\VE482\Lab\Lab 6\WuQinhang Lab6 Report.assets\image-20201107004344572.png)

After the rebuild:

![image-20201108231629114](C:\Users\William Wu\Documents\Mainframe Files\UMJI-SJTU\1 Academy\20 Fall\VE482\Lab\Lab 6\WuQinhang Lab6 Report.assets\image-20201108231629114.png)

## Plugin Architecture Design

### architecture design[^6]

See `./cmplx-sort/*` for details.

Skeleton of the plugin:

```c
// plugin.h (kernel)
typedef struct _Handle{
  void *handle; // handle for dynamic loading functions
  char type[36]; // name of the file type
  // void (*sort)(Type*, List*); // function pointer for sort
  void (*ui_cmd)(char**); // function pointer for ui_cmd
  // ...
} Handle;

int pluginInit(Handle **handle);
```

```c
// ui.h/c (csv-plugin)
#include "../src-kernel/plugin-api.h"
void ui_cmd(char *argv[]){
  // ...
}
```

### plugin-csv implementation[^7] [^8] [^9]

The implementation of the csv file plugin is completed. See `./cmplx-sort/*` for details. A makefile and simple testcases are included.

Test result:

![image-20201108232651033](C:\Users\William Wu\Documents\Mainframe Files\UMJI-SJTU\1 Academy\20 Fall\VE482\Lab\Lab 6\WuQinhang Lab6 Report.assets\image-20201108232651033.png)



[^1]:Zathura Source Code, *GitLab*, 2020. https://git.pwmt.org/pwmt/zathura (accessed Nov. 06, 2020).
[^2]: Zathura Overview, *Pwmt.org*, 2020. https://pwmt.org/projects/zathura/ (accessed Nov. 06, 2020).
[^3]: Zathura Plugin Documentation,  *Pwmt.org*, 2020. https://pwmt.org/projects/zathura/plugins/development/ (accessed Nov. 06, 2020).
[^4]: Zathura-cb Source Code, *GitLab*, 2020. https://git.pwmt.org/pwmt/zathura-cb (accessed Nov. 06, 2020).
[^5]: Zathura-cb Documentation, *Pwmt.org*, 2020. https://pwmt.org/projects/zathura-cb/ (accessed Nov. 06, 2020).

[^6]:Sven Gregori, “It’s All In The Libs – Building A Plugin System Using Dynamic Loading,” *Hackaday*, Jul. 12, 2018. https://hackaday.com/2018/07/12/its-all-in-the-libs-building-a-plugin-system-using-dynamic-loading/ (accessed Nov. 07, 2020).
[^7]:samoz, “How can I get the list of files in a directory using C or C++?,” *Stack Overflow*, Mar. 04, 2009. https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c (accessed Nov. 07, 2020).‌
[^8]:T. ivory, “ISO C Void * and Function Pointers,” *Stack Overflow*, Jan. 03, 2013. https://stackoverflow.com/questions/14134245/iso-c-void-and-function-pointers (accessed Nov. 07, 2020).
[^9]:“strsep(3) - Linux manual page,” *Man7.org*, 2019. https://man7.org/linux/man-pages/man3/strsep.3.html (accessed Nov. 07, 2020).
[^10]: J. Bodnar, “Text in Cairo,” *Zetcode.com*, 2020. http://zetcode.com/gfx/cairo/cairotext/ (accessed Nov. 08, 2020).
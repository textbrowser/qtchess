/*
** Copyright (c) 2003, 2004, 2006, 2007, 2008 Alexis Megas
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
** -- Local Includes --
*/

#include "qtchess.h"

/*
** -- Declarations --
*/

void cleanup(void);
void quit_program(int);

/*
** -- Globals --
*/

qtchess *chess = 0;
qtchess_gui *gui = 0;
qtchess_comm *comm = 0;
QApplication *qapp = 0;

/*
** -- Main --
*/

#ifndef QTCHESS_PLUGIN

int main(int argc, char *argv[])
{
  int rc = 0;

  QApplication::setColorSpec(QApplication::CustomColor);

  if((qapp = new(std::nothrow) QApplication(argc, argv)) == 0)
    {
      (void) fprintf(stderr, "Memory allocation failure.\n");
      return EXIT_FAILURE;
    }

  if((chess = new(std::nothrow) qtchess()) == 0)
    {
      (void) fprintf(stderr, "Memory allocation failure.\n");
      return EXIT_FAILURE;
    }

  chess->init();

  if((gui = new(std::nothrow) qtchess_gui()) == 0)
    chess->quit("Memory allocation failure.", EXIT_FAILURE);

  if((comm = new(std::nothrow) qtchess_comm()) == 0)
    chess->quit("Memory allocation failure.", EXIT_FAILURE);

  /*
  ** Must be called early.
  */

  gui->init();

  if(!QGLFormat::hasOpenGL())
    {
      (void) fprintf(stderr, "OpenGL is not supported on this system.\n");
      return EXIT_FAILURE;
    }

  /*
  ** Initialize communications.
  */

  comm->init();

  /*
  ** Display the board according to its configuration.
  */

  gui->display();

  /*
  ** Exit.
  */

  rc = gui->exec();
  cleanup();
  return rc;
}

#else

#endif

void cleanup(void)
{
  if(comm != 0)
    comm->quit();

  if(gui != 0)
    {
      delete gui;
      gui = 0;
    }

  if(chess != 0)
    {
      delete chess;
      chess = 0;
    }
}

void quit_program(int exit_code)
{
  /*
  ** Perform some cleanup.
  */

  cleanup();
  exit(exit_code);
}

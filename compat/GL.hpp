/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#ifdef USE_OPENGL_2

#define xglBindBuffer glBindBuffer
#define xglBufferData glBufferData
#define xglGenBuffers glGenBuffers

#else

#include "client/common/Utils.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h> // it'll include from a different dir, namely thirdparty/GL/glext.h

void xglInit();
bool xglInitted();

void xglBindBuffer(GLenum target, GLuint buffer);
void xglBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
void xglGenBuffers(GLsizei num, GLuint* buffers);
void xglDeleteBuffers(GLsizei num, GLuint* buffers);
void xglOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearpl, GLfloat farpl);
void xglSwapIntervalEXT(int interval);

// @TODO: not the right place, but er, it's ok
void drawArrayVT(GLuint buffer, int count, int stride);
void drawArrayVTC(GLuint buffer, int count, int stride);

// According to the v0.1.0 touch prototype.

#ifndef ORIGINAL_CODE
//#define GL_DEBUG
#endif

#ifdef GL_DEBUG

// this sucks.....
#include "Options.hpp"

#define glDisable(cap)             do { if (Options::debugGl) { LOGI("glDisable @ %s:%d : %d\n",      __FILE__, __LINE__, cap);     } glDisable(cap);                   } while (0)
#define glEnable(cap)              do { if (Options::debugGl) { LOGI("glEnable @ %s:%d : %d\n",       __FILE__, __LINE__, cap);     } glEnable(cap);                    } while (0)
#define glPushMatrix()             do { if (Options::debugGl) { LOGI("glPushM @ %s:%d\n",             __FILE__, __LINE__);          } glPushMatrix();                   } while (0)
#define glPopMatrix()              do { if (Options::debugGl) { LOGI("glPopM  @ %s:%d\n",             __FILE__, __LINE__);          } glPopMatrix();                    } while (0)
#define glTranslate(x,y,z)         do { if (Options::debugGl) { LOGI("glTrans @ %s:%d: %f,%f,%f\n",   __FILE__, __LINE__,x,y,z);    } glTranslate(x,y,z);               } while (0)
#define glRotate(an,x,y,z)         do { if (Options::debugGl) { LOGI("glRotat @ %s:%d: %f,%f,%f,%f\n",__FILE__, __LINE__,an,x,y,z); } glRotate(an,x,y,z);               } while (0)
#define glLoadIdentity()           do { if (Options::debugGl) { LOGI("glLoadI @ %s:%d\n",             __FILE__, __LINE__);          } glLoadIdentity();                 } while (0)
#define glEnableClientState(c)     do { if (Options::debugGl) { LOGI("glEnableClient @ %s:%d : %d\n", __FILE__, __LINE__, c);       } glEnableClientState(c);           } while (0)
#define glDisableClientState(c)    do { if (Options::debugGl) { LOGI("glDisableClient @ %s:%d : %d\n",__FILE__, __LINE__, c);       } glDisableClientState(c);          } while (0)
#define xglGenBuffers(a,b)         do { if (Options::debugGl) { LOGI("glGenBuffers @ %s:%d : %d\n",   __FILE__, __LINE__, b);       } xglGenBuffers(a,b);               } while (0)
#define xglBindBuffer(targ,buf)    do { if (Options::debugGl) { LOGI("glBindBuffer @ %s:%d : %d\n",   __FILE__, __LINE__, buf);     } xglBindBuffer(targ, buf);         } while (0)
#define glVertexPointer(a,b,c,d)   do { if (Options::debugGl) { LOGI("glVertexPtr @ %s:%d : %d\n",    __FILE__, __LINE__, 0);       } glVertexPointer(a,b,c,d);         } while (0) //! These appear to print straight-up zero?
#define glTexCoordPointer(a,b,c,d) do { if (Options::debugGl) { LOGI("glTexPtr @ %s:%d : %d\n",       __FILE__, __LINE__, 0);       } glTexCoordPointer(a,b,c,d);       } while (0)
#define glColorPointer(a,b,c,d)    do { if (Options::debugGl) { LOGI("glColorPtr @ %s:%d : %d\n",     __FILE__, __LINE__, 0);       } glColorPointer(a,b,c,d);          } while (0)
#define glDrawArrays(a,b,c)        do { if (Options::debugGl) { LOGI("glDrawA @ %s:%d : %d\n",        __FILE__, __LINE__, b);       } glDrawArrays(a,b,c);              } while (0)
#define glTexImage2D(...)          do { if (Options::debugGl) { LOGI("glTexImage2D @ %s:%d : %d\n",   __FILE__, __LINE__, 0);       } glTexImage2D(__VA_ARGS__);        } while (0)
#define glTexSubImage2D(a,b,...)   do { if (Options::debugGl) { LOGI("glTexSubImage2D @ %s:%d : %d\n",__FILE__, __LINE__, b);       } glTexSubImage2D(a,b,__VA_ARGS__); } while (0)
#define glTexParameteri(a,b,c)     do { if (Options::debugGl) { LOGI("glTexParameteri @ %s:%d : %d\n",__FILE__, __LINE__, c);       } glTexParameteri(a,b,c);           } while (0)

#endif

#endif

/*****************************************************************************
 *   Preliminary Qt test code for v3dfxbase (QGLWidget derived)
 *
 * Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *   
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *   
 *   Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Contact: prabu@ti.com
 ****************************************************************************/

#include "videowidget.h"

#include <QtGui>
#include <QGraphicsScene>
#include <QPainter>
#include <QtCore/QtDebug>
#include <QtCore/QTimer>


static int curritercount = 0;

extern int qt_program_setup(int testID);
extern void qt_program_cleanup(int testID);
extern int allocate_v3dfx_imgstream_bufs(int numbufs);
extern void deallocate_v3dfx_imgstream_bufs();
extern void test8();
extern void test20_init();
extern void test20_process_one(int currIteration);
extern void test20_deinit();

extern void test3_init();
extern void test3_process_one(int currIteration);
extern void test3_deinit();

VideoWidget::VideoWidget(QGLWidget *parent)
:V3dfxGLWidget(parent)
{
	qWarning() << __func__ << "VideoWidget constructor called";

	//For testing, comment out
	//V3dfxGLItem::init();

	currTimer = new QTimer(this);
	connect(currTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
	currTimer->start(1000);
}

//TODO separate application code and base code from init
int VideoWidget::init()
{
	int err;

	qWarning() << __func__ << "VideoWidget called";
	qWarning() << curritercount;

#if 1
	err = qt_program_setup(8);
	qWarning() << __func__ << "1";
	if(err) 
	{
		qt_program_cleanup(8);		
		goto completed;
	}
	/* GL_IMG_texture_stream - via v3dfxbase */
	allocate_v3dfx_imgstream_bufs(2); //2 buffers
	qWarning() << __func__ << "2";
	test20_init();
	qWarning() << __func__ << "3";
#else
	test3_init();
#endif
	qWarning() << __func__ << "init complete";

completed:
	return 0;
}

void VideoWidget::deinit()
{
		qWarning() << __func__ << "5 deinit";
#if 1
		test20_deinit();
		deallocate_v3dfx_imgstream_bufs();
		qt_program_cleanup(8);
#else
		test3_deinit();
#endif
}


void VideoWidget::updateGL()
{
	qWarning() << __func__ << "VideoWidget called";
	qWarning() << __func__ << "4, with iteration #" << curritercount;
	curritercount ++;
	V3dfxGLWidget::updateGL();
}

void VideoWidget::paintGL()
{
	qWarning() << __func__ << "VideoWidget called";
	QString frameString;
	QTextStream(&frameString) << "frame count = " << curritercount;

	QPainter painter;
	painter.begin(this);
	painter.beginNativePainting();

#if 1
	test20_process_one(curritercount);
#else
	test3_process_one(curritercount);
#endif

	painter.drawText(20, 40, frameString);
	painter.end();
    
	V3dfxGLWidget::paintGL();
}

void VideoWidget::initializeGL()
{
	qWarning() << __func__ << "VideoWidget called";
	init();
	V3dfxGLWidget::initializeGL();
}

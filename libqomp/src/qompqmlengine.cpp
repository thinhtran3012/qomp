/*
 * Copyright (C) 2014  Khryukin Evgeny
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "qompqmlengine.h"

#include <QCoreApplication>
#include <QQuickItem>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlProperty>
#include <QQuickWindow>
#include <QQuickImageProvider>
#include <QPixmapCache>

#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>
#include <QKeyEvent>
#endif

QQuickWindow* _instance = nullptr;

#ifdef Q_OS_ANDROID
static void menuKeyDown(JNIEnv */*env*/, jobject /*thiz*/)
{
	qApp->postEvent(_instance->activeFocusItem(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_Menu, Qt::NoModifier));//QMetaObject::invokeMethod(_instance, "doMainMenu");
}
#endif

class QompImageProvider : public QQuickImageProvider
{
public:
	QompImageProvider() : QQuickImageProvider(QQmlImageProviderBase::Pixmap){}

	virtual QPixmap	requestPixmap(const QString &id, QSize */*size*/, const QSize &/*requestedSize*/)
	{
		QPixmap pix;
		QPixmapCache::find(id, &pix);
		return pix;
	}

	static const QString& name()
	{
		static const QString n = QStringLiteral("qomp");
		return n;
	}
};


QompQmlEngine *QompQmlEngine::instance()
{
	if(!instance_)
		instance_ = new QompQmlEngine;

	return instance_;
}

QompQmlEngine::~QompQmlEngine()
{
//	QMetaObject::invokeMethod(window_, "clear", Qt::DirectConnection);

	removeImageProvider(QompImageProvider::name());
}

QQuickItem *QompQmlEngine::createItem(const QUrl &url)
{
	QQmlContext* context = new QQmlContext(this);
	QQmlComponent* comp = new QQmlComponent(this, url);
	QQuickItem* item = static_cast<QQuickItem*>(comp->create(context));
	context->setParent(item);
	comp->setParent(item);
	connect(item, SIGNAL(destroyed()), SLOT(itemDeleted()));
	return item;
}

void QompQmlEngine::addItem(QQuickItem *item)
{
	item->setParent(0);
	setObjectOwnership(item, QQmlEngine::JavaScriptOwnership);
	QMetaObject::invokeMethod(window_, "addView", Qt::DirectConnection,
				  Q_ARG(QVariant, QVariant::fromValue(item)));
}

void QompQmlEngine::removeItem()
{
	QMetaObject::invokeMethod(window_, "removeView", Qt::DirectConnection);
}

void QompQmlEngine::itemDeleted()
{
#ifdef DEBUG_OUTPUT
	qDebug() << "QompQmlEngine::itemDeleted() " << sender()->metaObject()->className();
#endif
}

QompQmlEngine::QompQmlEngine() :
	QQmlApplicationEngine(qApp),
	window_(0)
{
	addImageProvider(QompImageProvider::name(), new QompImageProvider);

	load(QUrl("qrc:///qmlshared/QompAppWindow.qml"));
	window_ = static_cast<QQuickWindow*>(rootObjects().first());
	connect(window_, SIGNAL(exit()), SIGNAL(quit()));
	_instance = window_;

#ifdef Q_OS_ANDROID
	QAndroidJniObject act = QtAndroid::androidActivity();
	QAndroidJniEnvironment jni;
	jclass clazz = jni->GetObjectClass(act.object());
	JNINativeMethod methods[] = { "menuKeyDown", "()V", (void*)menuKeyDown };
	jni->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0]));
	jni->DeleteLocalRef(clazz);
#endif
}

QompQmlEngine* QompQmlEngine::instance_ = 0;

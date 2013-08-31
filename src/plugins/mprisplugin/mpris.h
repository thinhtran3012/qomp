/*
 * Copyright (C) 2013  Khryukin Evgeny, Vitaly Tonkacheyev
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
 
#ifndef MPRIS_H
#define MPRIS_H

#include <QtDBus/QDBusAbstractAdaptor>
#include <QVariantMap>

//class DBUSAdaptor;

class Mpris : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
	Q_PROPERTY(QVariantMap Metadata READ metadata)
    Q_PROPERTY(QString PlaybackStatus READ playbackStatus)
public:
    Mpris(QObject *p = 0);
public:
	QVariantMap metadata() const;
	QString playbackStatus() const;
	void setStatus(const QString &status);
	void setMetadata(int trackNumber, const QString &title, const QString &artist, const QString &album, const QString &url);
	void sendProperties();

private:
	QVariantMap metaData;
	QString playerStatus;
	bool statusChanged;
	bool metadataChanged;
};

#endif // MPRIS_H

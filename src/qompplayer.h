/*
 * Copyright (C) 2013  Khryukin Evgeny
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

#ifndef QOPMPLAYER_H
#define QOPMPLAYER_H

#include "tune.h"

namespace Phonon {
class SeekSlider;
class VolumeSlider;
class AudioOutput;
class MediaSource;
}

#include <Phonon/MediaObject>

class QompPlayer : public QObject
{
	Q_OBJECT
public:
	QompPlayer(QObject *parent = 0);
	~QompPlayer();

	enum State { StateUnknown = 0, StateStopped, StatePaused, StatePlaing, StateError, StateLoading };

	void setSeekSlider(Phonon::SeekSlider* slider);
	void setVolumeSlider(Phonon::VolumeSlider* slider);
	State state() const;
	void setTune(const Tune& tune);
	Tune currentTune() const;
	void playOrPause();
	void stop();

	void setAudioOutputDevice(int index);
	
signals:
	void currentPosition(qint64 pos);
	void stateChanged(QompPlayer::State newState);
	void mediaFinished();

private slots:
	void stateChanged();
	
private:
	Phonon::MediaObject* mediaObject_;
	Phonon::AudioOutput* audioOutput_;
	Phonon::AudioOutputDevice defaultDevice_;
	Tune currentTune_;
};

#endif // QOPMPLAYER_H

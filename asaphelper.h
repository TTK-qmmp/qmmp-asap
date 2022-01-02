/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2022 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef ASAPHELPER_H
#define ASAPHELPER_H

extern "C" {
#include <libasap/asap.h>
}
#include <QMap>
#include <QFile>
#include <qmmp/qmmp.h>

typedef struct {
    ASAP *input;
    int length;
    int bitrate;
    int channels;
} decode_info;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class AsapHelper
{
public:
    explicit AsapHelper(const QString &path);
    ~AsapHelper();

    void deinit();

    bool initialize();
    qint64 totalTime() const;
    void seek(qint64 time);

    int bitrate() const;
    int sampleRate() const;
    int channels() const;
    int bitsPerSample() const;

    qint64 read(unsigned char *data, qint64 maxSize);
    const QMap<Qmmp::MetaData, QString> &readMetaData() const;

private:
    QString m_path;
    decode_info *m_info = nullptr;
    QMap<Qmmp::MetaData, QString> m_metaData;

};

#endif

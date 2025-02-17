/*
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "settingsmodel.h"

SettingsModel::SettingsModel(QObject *parent) :
    QObject(parent)
{
    settings = new QSettings();

    setAudioCodec(audioCodec());
    setContainerFormat(containerFormat());
    setAudioQuality(audioQuality());
}

SettingsModel::~SettingsModel()
{
    delete settings;
}

int SettingsModel::simpleAudioFormat() const
{
    const auto keys = formatMap.keys();
    auto format_it = std::find_if(keys.begin(), keys.end(), [&](const SimpleAudioFormat p) -> bool {
        return formatMap[p].first == audioCodec() && formatMap[p].second == containerFormat();
    });

    if (format_it == keys.end()) {
        return SimpleAudioFormat::OTHER;
    }

    return *format_it;
}

void SettingsModel::setSimpleAudioFormat(int audioFormat)
{
    SimpleAudioFormat format = static_cast<SimpleAudioFormat>(audioFormat);
    setAudioCodec(formatMap[format].first);
    setContainerFormat(formatMap[format].second);
}

QString SettingsModel::audioCodec() const
{
    return settings->value("General/audioCodec", "audio/x-opus").toString();
}

void SettingsModel::setAudioCodec(const QString &audioCodec)
{
    AudioRecorder::instance()->setAudioCodec(audioCodec);
    settings->setValue("General/audioCodec", audioCodec);

    emit audioCodecChanged();
    emit simpleAudioFormatChanged();
}

QString SettingsModel::containerFormat() const
{
    return settings->value("General/containerFormat", "audio/ogg").toString();
}

void SettingsModel::setContainerFormat(const QString &audioContainerFormat)
{
    AudioRecorder::instance()->setContainerFormat(audioContainerFormat);
    settings->setValue("General/containerFormat", audioContainerFormat);

    emit containerFormatChanged();
    emit simpleAudioFormatChanged();
}

int SettingsModel::audioQuality() const
{
    return settings->value("General/audioQuality", 3).toInt();
}

void SettingsModel::setAudioQuality(int audioQuality)
{
    QAudioEncoderSettings s = AudioRecorder::instance()->audioSettings();
    s.setQuality(static_cast<QMultimedia::EncodingQuality>(audioQuality));
    AudioRecorder::instance()->setAudioSettings(s);
    int sampleRate = s.sampleRate();
    QString codec = s.codec();
    int count = s.channelCount();
    settings->setValue("General/audioQuality", audioQuality);

    emit audioQualityChanged();
}

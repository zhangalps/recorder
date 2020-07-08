#include "recordingmodel.h"

#include <QFile>
#include <QStandardPaths>
#include <QJsonObject>

#include "utils.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>

/* ~ Recording ~ */

Recording::Recording(QObject* parent, const QString &filePath, const QString &fileName, QDateTime recordDate, int recordingLength)
    : QObject(parent)
    , m_filePath(filePath)
    , m_fileName(fileName)
    , m_recordDate(recordDate)
    , m_recordingLength(recordingLength)
{}

Recording::Recording(const QJsonObject &obj)
{
    m_filePath = obj["filePath"].toString();
    m_fileName = obj["fileName"].toString();
    m_recordDate = QDateTime::fromString(obj["recordDate"].toString(), Qt::DateFormat::ISODate);
    m_recordingLength = obj["recordingLength"].toInt();
}

Recording::~Recording()
{
}


QJsonObject Recording::toJson()
{
    QJsonObject obj;
    obj["filePath"] = m_filePath;
    obj["fileName"] = m_fileName;
    obj["recordDate"] = m_recordDate.toString(Qt::DateFormat::ISODate);
    obj["recordingLength"] = m_recordingLength;
    return obj;
}


/* ~ RecordingModel ~ */

RecordingModel::RecordingModel(QObject *parent) : QAbstractListModel(parent)
{
    m_settings = new QSettings(parent);
    load();
}

RecordingModel::~RecordingModel()
{
    save();
    delete m_settings;
    
    for (auto *v : m_recordings)
        delete v;
}

void RecordingModel::load()
{
    QJsonDocument doc = QJsonDocument::fromJson(m_settings->value(QStringLiteral("recordings")).toString().toUtf8());
    for (QJsonValueRef r : doc.array()) {
        QJsonObject obj = r.toObject();
        m_recordings.append(new Recording(obj));
    }
}

void RecordingModel::save()
{
    QJsonArray arr;
    for (auto rec : m_recordings)
        arr.push_back(rec->toJson());
    
    m_settings->setValue(QStringLiteral("recordings"), QString(QJsonDocument(arr).toJson(QJsonDocument::Compact)));
}

QVariant RecordingModel::data(const QModelIndex &index, int role) const
{
    return {};
}

int RecordingModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_recordings.count();
}

Recording* RecordingModel::at(int index)
{
    if (index < 0 || index >= m_recordings.size())
        return {};
    return m_recordings.at(index);
}

void RecordingModel::insertRecording(QString filePath, QString fileName, QDateTime recordDate, int recordingLength)
{
    beginInsertRows({}, m_recordings.count(), m_recordings.count());
    m_recordings.append(new Recording(this, filePath, fileName, recordDate, recordingLength));
    endInsertRows();
    
    save();
}

void RecordingModel::deleteRecording(const int index)
{
    QFile::remove(m_recordings[index]->filePath());
    beginRemoveRows({}, index, index);
    m_recordings.removeAt(index);
    endRemoveRows();
    
    save();
}


#include "asaphelper.h"

AsapHelper::AsapHelper(const QString &path)
    : m_path(path)
{
    m_info = (decode_info*)calloc(sizeof(decode_info), 1);
}

AsapHelper::~AsapHelper()
{
    deinit();
}

void AsapHelper::deinit()
{
    if(m_info)
    {
        if(m_info->input)
        {
            ASAP_Delete(m_info->input);
        }
        free(m_info);
    }
}

bool AsapHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("AsapHelper: open file failed");
        return false;
    }

    const qint64 size = file.size();
    const QByteArray module = file.readAll();

    m_info->input = ASAP_New();
    ASAP_DetectSilence(m_info->input, 5);

    if(!ASAP_Load(m_info->input, qPrintable(m_path), (unsigned char *)module.constData(), size))
    {
        qWarning("AsapHelper: ASAP_Load error");
        return false;
    }

    ASAPInfo *info =(ASAPInfo *)ASAP_GetInfo(m_info->input);
    //struct ASAPInfo { int channels; int covoxAddr; int defaultSong; int fastplay; int headerLen;
    // int init; int music; cibool ntsc; int player; int songs; ASAPModuleType type;
    // unsigned char songPos[32]; char author[128]; char date[128]; int durations[32];
    // char filename[128]; cibool loops[32]; char title[128]; };
    if(!ASAP_PlaySong(m_info->input, ASAPInfo_GetDefaultSong(info), 360000))
    {
        qWarning("AsapHelper: ASAP_PlaySong error");
        return false;
    }

    m_metaData.insert(Qmmp::TITLE, ASAPInfo_GetTitle(info));
    m_metaData.insert(Qmmp::ARTIST, ASAPInfo_GetAuthor(info));
    m_metaData.insert(Qmmp::YEAR, QString::number(ASAPInfo_GetYear(info)));

    m_info->length = ASAPInfo_GetDuration(info, ASAPInfo_GetDefaultSong(info));
    m_info->channels = ASAPInfo_GetChannels(info);
    m_info->bitrate = size * 8.0 / m_info->length + 1.0f;
    return true;
}

int AsapHelper::totalTime() const
{
    return m_info->length;
}

void AsapHelper::seek(qint64 time)
{
    ASAP_Seek(m_info->input, time);
}

int AsapHelper::bitrate() const
{
    return m_info->bitrate;
}

int AsapHelper::sampleRate() const
{
    return ASAP_SAMPLE_RATE;
}

int AsapHelper::channels() const
{
    return m_info->channels;
}

int AsapHelper::bitsPerSample() const
{
    return 16;
}

int AsapHelper::read(unsigned char *buf, int size)
{
    if(ASAP_GetPosition(m_info->input) >= totalTime())
    {
        return 0;
    }

    return ASAP_Generate(m_info->input, buf, size, ASAPSampleFormat_S16_L_E);
}

const QMap<Qmmp::MetaData, QString> &AsapHelper::readMetaData() const
{
    return m_metaData;
}


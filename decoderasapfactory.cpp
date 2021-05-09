#include "asaphelper.h"
#include "decoder_asap.h"
#include "decoderasapfactory.h"
#include "asapmetadatamodel.h"

#include <QMessageBox>

bool DecoderAsapFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderAsapFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Asap Plugin");
    properties.filters << "*.sap";
    properties.filters << "*.cm3" << "*.cmc" << "*.cmr" << "*.cms";
    properties.filters << "*.dmc" << "*.dlt";
    properties.filters << "*.mpd" << "*.mpt" << "*.rmt" << "*.tm2" << "*.tm8" << "*.tmc";
    properties.description = tr("Another Slight Atari Player Files");
    properties.shortName = "asap";
    properties.noInput = true;
    properties.protocols << "asap";
    return properties;
}

Decoder *DecoderAsapFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderAsap(path);
}

QList<TrackInfo*> DecoderAsapFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    AsapHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        info->setValue(Qmmp::TITLE, helper.title());
        info->setValue(Qmmp::ARTIST, helper.artist());
        info->setValue(Qmmp::YEAR, helper.year());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.bitsPerSample());
        info->setValue(Qmmp::FORMAT_NAME, "Asap");
        info->setDuration(helper.totalTime());
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderAsapFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new AsapMetaDataModel(path);
}

void DecoderAsapFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

void DecoderAsapFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About Asap Reader Plugin"),
                        tr("Qmmp Asap Reader Plugin")+"\n"+
                        tr("Written by: Greedysky <greedysky@163.com>"));
}

QString DecoderAsapFactory::translation() const
{
    return QString();
}

#include "decoderasapfactory.h"
#include "asaphelper.h"
#include "decoder_asap.h"
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
    properties.shortName = "asap";
    properties.filters << "*.cm3" << "*.cmc" << "*.cmr" << "*.cms";
    properties.filters << "*.dlt" << "*.dmc";
    properties.filters << "*.fc";
    properties.filters << "*.mpd" << "*.mpt" << "*.rmt";
    properties.filters << "*.sap";
    properties.filters << "*.tm2" << "*.tm8" << "*.tmc";
    properties.description = "Another Slight Atari Player File";
    properties.noInput = true;
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
        const QMap<Qmmp::MetaData, QString> metaData(helper.readMetaData());
        for(auto itr = metaData.begin(); itr != metaData.end(); ++itr)
        {
            info->setValue(itr.key(), itr.value());
        }
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
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

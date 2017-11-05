#include "IMVideoCtrl.h"

IMVideoCtrl::IMVideoCtrl(const QString & devName, QObject *parent) :
    QObject(parent)
{
    m_devName = devName;
    m_fd = -1;
    m_buffers = NULL;
    m_nBuffers = 0;
    m_index = -1;
}

IMVideoCtrl::~IMVideoCtrl()
{
    stopCapturing();
    uninitDevice();
    closeDevice();
    free(m_buffers);
}

int IMVideoCtrl::openDevice()
{
    m_fd = open(m_devName.toStdString().c_str(), O_RDWR/*|O_NONBLOCK*/, 0);

    if(-1 == m_fd)
    {
        emit displayError(tr("open device: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int IMVideoCtrl::closeDevice()
{
    if(-1 == close(m_fd))
    {
        emit displayError(tr("close: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int IMVideoCtrl::initDevice()
{
    return 0;
}


int IMVideoCtrl::initMmap()
{
    return 0;
}

int IMVideoCtrl::startCapturing()
{
    return 0;
}


int IMVideoCtrl::stopCapturing()
{
    return 0;
}

int IMVideoCtrl::uninitDevice()
{
    return 0;
}

int IMVideoCtrl::getFrame(void **frame_buf, size_t* len)
{
    return 0;
}

int IMVideoCtrl::ungetFrame()
{
    return 0;
}

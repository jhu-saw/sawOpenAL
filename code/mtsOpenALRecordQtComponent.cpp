/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s): Marcin Balicki
  Created on: 2011-02-10

  (C) Copyright 2011 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QStringList>
#include <QList>
#include <QFileDialog>

#include <cisstMultiTask/mtsVector.h> // for mtsStdStringVec
#include <cisstMultiTask/mtsTaskManager.h>
#include <cisstMultiTask/mtsInterfaceRequired.h>

#include <sawOpenAL/mtsOpenALRecordQtComponent.h>

CMN_IMPLEMENT_SERVICES(mtsOpenALRecordQtComponent);

mtsOpenALRecordQtComponent::mtsOpenALRecordQtComponent(const std::string & name, double period):
    mtsComponent(name),
    Time(0),
    PlayStartTime(0)
{

    startTimer(period * 1000);

    ErrorMessageDialog = new QErrorMessage();

    // create the user interface
    RecWidget.setupUi(&Widget);

    // create the widgets
    Plot        = new vctPlot2DOpenGLQtWidget(&Widget);
    DataTrace   = Plot->AddTrace("Data");

    RecWidget.MainVLayout->addWidget(Plot);

    TimeServer = mtsTaskManager::GetInstance()->GetTimeServer();

    mtsInterfaceRequired* required = AddInterfaceRequired("RequiresAudioRecorder", MTS_OPTIONAL);
    if (required) {
        required->AddFunction("GetIsRecording", Recorder.GetIsRecording);
        required->AddFunction("Start",          Recorder.Start);
        required->AddFunction("Stop",           Recorder.Stop);
        required->AddFunction("SetFileName",    Recorder.SetFileName);
        required->AddFunction("SetCaptureDeviceName",   Recorder.SetCaptureDeviceName);
        required->AddFunction("SetCaptureDeviceID",     Recorder.SetCaptureDeviceID);
        required->AddFunction("GetCaptureDeviceNames",  Recorder.GetCaptureDeviceNames);
        required->AddFunction("GetStreamVolume",        Recorder.GetStreamVolume);
        required->AddFunction("GetTime",                Recorder.GetTime);
        required->AddFunction("GetFileSize",            Recorder.GetFileSize);
    }
}


void mtsOpenALRecordQtComponent::MakeQTConnections(void)
{
    QObject::connect(RecWidget.RecordButton, SIGNAL(clicked()),
                     this, SLOT(QSlotRecordClicked()));

    QObject::connect(RecWidget.StopButton, SIGNAL(clicked()),
                     this, SLOT(QSlotStopClicked()));

    QObject::connect(RecWidget.PathButton, SIGNAL(clicked(void)),
                     this, SLOT(QSlotPathClicked(void)));

    QObject::connect(RecWidget.DeviceListComboBox, SIGNAL(activated(const QString&)),
                     this, SLOT(QSlotDeviceSelected(const QString&)));

}


void mtsOpenALRecordQtComponent::Configure(const std::string & CMN_UNUSED(filename))
{
    MakeQTConnections();
    //    //! /todo Temporary hack, remove
    //    // DataEndTime     = 300;
    //    UpdateLimits();
    //    PlayUntilTime = DataStartTime;

    //Widget.resize(700,300);
    Widget.show();
    mtsStdStringVec names;
    Recorder.GetCaptureDeviceNames(names);
    CMN_LOG_CLASS_RUN_VERBOSE << names.size() << " CaptureDeviceNames are: " << std::endl;

    QStringList qnameList;

    for (unsigned int i = 0; i< names.size(); i ++) {
        qnameList.push_back(QString::fromStdString(names[i]));
        CMN_LOG_CLASS_RUN_VERBOSE << i << ":" << names[i] << std::endl;
    }
    RecWidget.DeviceListComboBox->addItems(qnameList);

}


void mtsOpenALRecordQtComponent::ErrorMessage(const std::string & message)
{
    //    ErrorMessageDialog->showMessage(tr(msg.c_str()));
    int ret = QMessageBox::critical(this->GetWidget(), tr(GetName().c_str()), tr(message.c_str()));
}


void mtsOpenALRecordQtComponent::QSlotRecordClicked()
{
    CMN_LOG_CLASS_RUN_VERBOSE << "QSlotRecordClicked: " << RecWidget.FileNameLineEdit->text().toStdString() << std::endl;
    Recorder.SetFileName(mtsStdString(RecWidget.PathLineEdit->text().toStdString()
                                      + std::string("/")
                                      + RecWidget.FileNameLineEdit->text().toStdString()));
    Recorder.Start();
}


void mtsOpenALRecordQtComponent::QSlotStopClicked()
{
    Recorder.Stop();
}


void mtsOpenALRecordQtComponent::QSlotPathClicked(void)
{
    QString pathName = QFileDialog::getExistingDirectory(&Widget, tr("Select Path"), tr("./"));
    RecWidget.PathLineEdit->setText(pathName);
}


void mtsOpenALRecordQtComponent::timerEvent(QTimerEvent * CMN_UNUSED(event))
{
    //RecWidget.TimeLabel->setText(QString::number(Time.Data,'f', 3));
    mtsBool isRecording;
    Recorder.GetIsRecording(isRecording);
    if (isRecording) {
        RecWidget.IsRecordingLabel->setStyleSheet("QLabel { background-color : red; color : white; }");
        RecWidget.IsRecordingLabel->setText("ON AIR");
    }
    else {
        RecWidget.IsRecordingLabel->setStyleSheet("QLabel { background-color : grey; color : black; }");
        RecWidget.IsRecordingLabel->setText("OFF AIR");
    }

    mtsDouble v;
    Recorder.GetStreamVolume(v);
    RecWidget.VolumeProgressBar->setValue(v.Data * 100);
    DataTrace->AddPoint(vctDouble2(v.Timestamp(), v.Data));

    Recorder.GetTime(v);
    RecWidget.TimeLabel->setText(QString::number(v.Data,'f', 3));

    Recorder.GetFileSize(v);
    RecWidget.SizeLabel->setText(QString::number(v.Data / 1000000.0, 'f', 1));

    Plot->updateGL();
}


void mtsOpenALRecordQtComponent::QSlotDeviceSelected(const QString & str)
{
    CMN_LOG_CLASS_RUN_VERBOSE << "SetCaptureDeviceName Clicked" << str.toStdString() << std::endl;
    Recorder.SetCaptureDeviceName(mtsStdString(str.toStdString()));
}

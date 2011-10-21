/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: osaOpenAL.h 3069 2011-10-18 14:25:14Z adeguet1 $

  Author(s):  Marcin Balicki
  Created on: 2011

  (C) Copyright 2011 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#ifndef _osaOpenAL_h
#define _osaOpenAL_h

#include <al.h>
#include <alc.h>

#include <cisstMultiTask/mtsTaskPeriodic.h>

// forward declaration of header classes
struct osaOpenALCAIHeader;
struct osaOpenALWAVHeader;

// Always include last!
#include <sawOpenAL/sawOpenALExport.h>

class osaOpenAL: public cmnGenericObject {
    // used to control the log level
    CMN_DECLARE_SERVICES(CMN_NO_DYNAMIC_CREATION, CMN_LOG_ALLOW_ERRORS_AND_WARNINGS);

protected:
    enum FileType {CAI, WAV};

    mtsStdString    FileName;
    FILE *          SoundFile;
    FileType        FType;

    char *          Data;
    unsigned int    NumDataBytes;

    osaOpenALCAIHeader * SoundSettings;

    ALuint          SoundBuffer[1];
    ALuint          SoundSource[1];

    osaOpenALCAIHeader * CAIHeader;
    osaOpenALWAVHeader * WAVHeader;

    mtsDouble       Time;
    mtsDouble       StartTimeAbsolute;
    mtsDouble       LengthInSec;

    mtsBool         IsPlaying;
    mtsDouble       Volume;
    mtsDouble       StreamVolume;

    double CalcStreamVolume(int samplePos);
    double CalcStreamTime(int samplePos);

    std::string GetALErrorString(ALenum err);
    bool CheckALError(std::string & error);

    const osaTimeServer * TimeServer;

    bool InitOpenAL(void);
    bool CloseOpenAL(void);

    ALCdevice * Device;
    ALCcontext * Context;

    mtsFunctionVoid RangeChangedEvent;

 public:
    enum SoundFormat {MONO8, MONO16, STEREO8, STEREO16};

    osaOpenAL();
    ~osaOpenAL();
    // all four methods are pure virtual in mtsTask
    inline void Configure(const std::string & CMN_UNUSED(filename)) {};
    void Startup(void);    // set some initial values
    void Play(void);
    void Pause(void);
    void Stop(void);
    void Run(void); // performed periodically
    void OpenFile(const mtsStdString & fName);
    //A the moment time is local, i.e. 0 is the start of the audio file.
    void Seek(const mtsDouble & time);
    // 0.0 - 1.0
    void SetVolume(const mtsDouble & volume);
    int  CalcStreamPos(double time);

    double GetStartTime();
    double GetEndTime();
    mtsBool   GetIsPlaying() { return IsPlaying;}
    mtsDouble GetTime() { return Time;}
    void GetStreamVolume(mtsDouble &volume) { volume = StreamVolume; }

    //converts cisst wav format from 2010 to standard wav format
   // static bool ConvertWAV10toWAV11(const std::string &inputFileName, const std::string &outputFileName);
};

CMN_DECLARE_SERVICES_INSTANTIATION(osaOpenAL);

#endif

// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#pragma once

class CMidiPerformanceManager
{
public:

    CMidiPerformanceManager() {}
    ~CMidiPerformanceManager() {}
    
    HRESULT Initialize();
    HRESULT Cleanup();

private:

};


// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include <string>
#include <istream>
#include "BaseServerRouteSettings.h"


namespace ofx {
namespace HTTP {


class ServerUploadRouteSettings : public BaseServerRouteSettings {
public:
    ServerUploadRouteSettings(const std::string& route = DEFAULT_ROUTE);
    virtual ~ServerUploadRouteSettings();

    bool        bRequireUploadFolderInDataFolder;
    std::string uploadFolder;
    bool        bAutoCreateUploadFolder;
    std::string uploadRedirect;
    size_t      writeBufferSize;

    static const std::string DEFAULT_ROUTE;
    static const std::string DEFAULT_UPLOAD_FOLDER;
    static const std::string DEFAULT_UPLOAD_REDIRECT;
    static const size_t      DEFAULT_BUFFER_SIZE;

};


} } // namespace ofx::HTTP
/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
// "liveMedia"
// Copyright (c) 1996-2010 Live Networks, Inc.  All rights reserved.
// A generic RTSP client - for a single "rtsp://" URL
// C++ header

#ifndef _RTSP_CLIENT_HH
#define _RTSP_CLIENT_HH

#ifndef _MEDIA_SESSION_HH
#include "MediaSession.hh"
#endif
#ifndef _NET_ADDRESS_HH
#include "NetAddress.hh"
#endif
#ifndef _DIGEST_AUTHENTICATION_HH
#include "DigestAuthentication.hh"
#endif

#define RTSPCLIENT_SYNCHRONOUS_INTERFACE 1 // For now, continue to support the old synchronous interface as well

class RTSPClient: public Medium {
public:
  static RTSPClient* createNew(UsageEnvironment& env, char const* rtspURL,
			       int verbosityLevel = 0,
			       char const* applicationName = NULL,
			       portNumBits tunnelOverHTTPPortNum = 0);
  // If "tunnelOverHTTPPortNum" is non-zero, we tunnel RTSP (and RTP)
  // over a HTTP connection with the given port number, using the technique
  // described in Apple's document <http://developer.apple.com/documentation/QuickTime/QTSS/Concepts/chapter_2_section_14.html>

  typedef void (responseHandler)(RTSPClient* rtspClient,
				 int resultCode, char* resultString);
      // A function that is called in response to a RTSP command.  The parameters are as follows:
      //     "rtspClient": The "RTSPClient" object on which the original command was issued.
      //     "resultCode": If zero, then the command completed successfully.  If non-zero, then the command did not complete
      //         successfully, and "resultCode" indicates the error, as follows:
      //             A positive "resultCode" is a RTSP error code (for example, 404 means "not found")
      //             A negative "resultCode" indicates a socket/network error; 0-"resultCode" is the standard "errno" code.
      //     "resultString": A ('\0'-terminated) string returned along with the response, or else NULL.
      //         In particular:
      //             "resultString" for a successful "DESCRIBE" command will be the media session's SDP description.
      //             "resultString" for a successful "OPTIONS" command will be a list of allowed commands.
      //         Note that this string can be present (i.e., not NULL) even if "resultCode" is non-zero - i.e., an error message.
      //         Note also that this string is dynamically allocated, and must be freed by the handler (or the caller)
      //             - using "delete[]".

  unsigned sendDescribeCommand(responseHandler* responseHandler, Authenticator* authenticator = NULL);
      // Issues a RTSP "DESCRIBE" command, then returns the "CSeq" sequence number that was used in the command.
      // The (programmer-supplied) "responseHandler" function is called later to handle the response
      //     (or is called immediately - with an error code - if the command cannot be sent).
      // "authenticator" (optional) is used for access control.  If you have username and password strings, you can use this by
      //     passing an actual parameter that you created by creating an "Authenticator(username, password) object".
      //     (Note that if you supply a non-NULL "authenticator" parameter, you need do this only for the first command you send.)

  unsigned sendOptionsCommand(responseHandler* responseHandler, Authenticator* authenticator = NULL);
      // Issues a RTSP "OPTIONS" command, then returns the "CSeq" sequence number that was used in the command.
      // (The "responseHandler" and "authenticator" parameters are as described for "sendDescribeCommand".)

  unsigned sendAnnounceCommand(char const* sdpDescription, responseHandler* responseHandler, Authenticator* authenticator = NULL);
      // Issues a RTSP "ANNOUNCE" command (with "sdpDescription" as parameter),
      //     then returns the "CSeq" sequence number that was used in the command.
      // (The "responseHandler" and "authenticator" parameters are as described for "sendDescribeCommand".)

  unsigned sendSetupCommand(MediaSubsession& subsession, responseHandler* responseHandler,
			    Boolean streamOutgoing = False,
			    Boolean streamUsingTCP = False,
			    Boolean forceMulticastOnUnspecified = False,
			    Authenticator* authenticator = NULL);
      // Issues a RTSP "SETUP" command, then returns the "CSeq" sequence number that was used in the command.
      // (The "responseHandler" and "authenticator" parameters are as described for "sendDescribeCommand".)

  unsigned sendPlayCommand(MediaSession& session, responseHandler* responseHandler,
			   double start = 0.0f, double end = -1.0f, float scale = 1.0f,
			   Authenticator* authenticator = NULL);
      // Issues an aggregate RTSP "PLAY" command on "session", then returns the "CSeq" sequence number that was used in the command.
      // (Note: start=-1 means 'resume'; end=-1 means 'play to end')
      // (The "responseHandler" and "authenticator" parameters are as described for "sendDescribeCommand".)
  unsigned sendPlayCommand(MediaSubsession& subsession, responseHandler* responseHandler,
			   double start = 0.0f, double end = -1.0f, float scale = 1.0f,
			   Authenticator* authenticator = NULL);
      // Issues a RTSP "PLAY" command on "subsession", then returns the "CSeq" sequence number that was used in the command.
      // (Note: start=-1 means 'resume'; end=-1 means 'play to end')
      // (The "responseHandler" and "authenticator" parameters are as described for "sendDescribeCommand".)

  unsigned sendPauseCommand(MediaSession& session, responseHandler* responseHandler, Authenticator* authenticator = NULL);
      // Issues an aggregate RTSP "PAUSE" command on "session", then returns the "CSeq" sequence number that was used in the command.
      // (The "responseHandler" and "authenticator" parameters are as described for "sendDescribeCommand".)
  unsigned sendPauseCommand(MediaSubsession& subsession, responseHandler* responseHandler, Authenticator* authenticator = NULL);
      // Issues a RTSP "PAUSE" command on "subsession", then returns the "CSeq" sequence number that was used in the command.
      // (The "responseHandler" and "authenticator" parameters are as described for "sendDescribeCommand".)

  unsigned sendRecordCommand(MediaSession& session, responseHandler* responseHandler, Authenticator* authenticator = NULL);
      // Issues an aggregate RTSP "RECORD" command on "session", then returns the "CSeq" sequence number that was used in the command.
      // (The "responseHandler" and "authenticator" parameters are as described for "sendDescribeCommand".)
  unsigned sendRecordCommand(MediaSubsession& subsession, responseHandler* responseHandler, Authenticator* authenticator = NULL);
      // Issues a RTSP "RECORD" command on "subsession", then returns the "CSeq" sequence number that was used in the command.
      // (The "responseHandler" and "authenticator" parameters are as described for "sendDescribeCommand".)

  unsigned sendTeardownCommand(MediaSession& session, responseHandler* responseHandler, Authenticator* authenticator = NULL);
      // Issues an aggregate RTSP "TEARDOWN" command on "session", then returns the "CSeq" sequence number that was used in the command.
      // (The "responseHandler" and "authenticator" parameters are as described for "sendDescribeCommand".)
  unsigned sendTeardownCommand(MediaSubsession& subsession, responseHandler* responseHandler, Authenticator* authenticator = NULL);
      // Issues a RTSP "TEARDOWN" command on "subsession", then returns the "CSeq" sequence number that was used in the command.
      // (The "responseHandler" and "authenticator" parameters are as described for "sendDescribeCommand".)

  unsigned sendSetParameterCommand(MediaSession& session, responseHandler* responseHandler,
				   char const* parameterName, char const* parameterValue,
				   Authenticator* authenticator = NULL);
      // Issues an aggregate RTSP "SET_PARAMETER" command on "session", then returns the "CSeq" sequence number that was used in the command.
      // (The "responseHandler" and "authenticator" parameters are as described for "sendDescribeCommand".)

  unsigned sendGetParameterCommand(MediaSession& session, responseHandler* responseHandler, char const* parameterName,
				   Authenticator* authenticator = NULL);
      // Issues an aggregate RTSP "GET_PARAMETER" command on "session", then returns the "CSeq" sequence number that was used in the command.
      // (The "responseHandler" and "authenticator" parameters are as described for "sendDescribeCommand".)

  Boolean changeResponseHandler(unsigned cseq, responseHandler* newResponseHandler);
      // Changes the response handler for the previously-performed command (whose operation returned "cseq").
      // (To turn off any response handling for the command, use a "newResponseHandler" value of NULL.  This might be done as part
      //  of an implementation of a 'timeout handler' on the command, for example.)
      // This function returns True iff "cseq" was for a valid previously-performed command (whose response is still unhandled).

  int socketNum() const { return fInputSocketNum; }

  static Boolean lookupByName(UsageEnvironment& env,
			      char const* sourceName,
			      RTSPClient*& resultClient);

  static Boolean parseRTSPURL(UsageEnvironment& env, char const* url,
			      NetAddress& address, portNumBits& portNum, char const** urlSuffix = NULL);
      // (ignores any "<username>[:<password>]@" in "url"); to get those, use:
  static Boolean parseRTSPURLUsernamePassword(char const* url,
					      char*& username, char*& password);

  void setUserAgentString(char const* userAgentName);
  // sets an alternative string to be used in RTSP "User-Agent:" headers

  unsigned sessionTimeoutParameter() const { return fSessionTimeoutParameter; }

  static unsigned responseBufferSize;

protected:
  RTSPClient(UsageEnvironment& env, char const* rtspURL,
	     int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum);
      // called only by createNew();
  virtual ~RTSPClient();

private: // redefined virtual functions
  virtual Boolean isRTSPClient() const;

public: // Some compilers complain if this is "private:"
  // The state of a request-in-progress:
  class RequestRecord {
  public:
    RequestRecord(unsigned cseq, char const* commandName, responseHandler* handler,
		  MediaSession* session = NULL, MediaSubsession* subsession = NULL, u_int32_t booleanFlags = 0,
		  double start = 0.0f, double end = -1.0f, float scale = 1.0f, char const* contentStr = NULL);
    virtual ~RequestRecord();

    RequestRecord*& next() { return fNext; }
    unsigned& cseq() { return fCSeq; }
    char const* commandName() const { return fCommandName; }
    MediaSession* session() const { return fSession; }
    MediaSubsession* subsession() const { return fSubsession; }
    u_int32_t booleanFlags() const { return fBooleanFlags; }
    double start() const { return fStart; }
    double end() const { return fEnd; }
    float scale() const { return fScale; }
    char* contentStr() const { return fContentStr; }
    responseHandler*& handler() { return fHandler; }

  private:
    RequestRecord* fNext;
    unsigned fCSeq;
    char const* fCommandName;
    MediaSession* fSession;
    MediaSubsession* fSubsession;
    u_int32_t fBooleanFlags;
    double fStart, fEnd;
    float fScale;
    char* fContentStr;
    responseHandler* fHandler;
  };
private:
  class RequestQueue {
  public:
    RequestQueue();
    virtual ~RequestQueue();

    void enqueue(RequestRecord* request); // "request" must not be NULL
    RequestRecord* dequeue();
    void putAtHead(RequestRecord* request); // "request" must not be NULL
    RequestRecord* findByCSeq(unsigned cseq);
    Boolean isEmpty() const { return fHead == NULL; }

  private:
    RequestRecord* fHead;
    RequestRecord* fTail;
  };

  void reset();
  void resetTCPSockets();
  void resetResponseBuffer();
  void setBaseURL(char const* url);
  int openConnection(); // -1: failure; 0: pending; 1: success
  int connectToServer(int socketNum, portNumBits remotePortNum); // used to implement "openConnection()"; result values are the same
  char* createAuthenticatorString(char const* cmd, char const* url);
  unsigned sendRequest(RequestRecord* request);
  void handleRequestError(RequestRecord* request);
  Boolean parseResponseCode(char const* line, unsigned& responseCode, char const*& responseString, Boolean& responseIsHTTP);
  void handleIncomingRequest();
  static Boolean checkForHeader(char const* line, char const* headerName, unsigned headerNameLength, char const*& headerParams);
  Boolean parseTransportParams(char const* paramsStr,
			       char*& serverAddressStr, portNumBits& serverPortNum,
			       unsigned char& rtpChannelId, unsigned char& rtcpChannelId);
  Boolean parseScaleParam(char const* paramStr, float& scale);
  Boolean parseRTPInfoParams(char const*& paramStr, u_int16_t& seqNum, u_int32_t& timestamp);
  Boolean handleSETUPResponse(MediaSubsession& subsession, char const* sessionParamsStr, char const* transportParamsStr,
			      Boolean streamUsingTCP);
  Boolean handlePLAYResponse(MediaSession& session, MediaSubsession& subsession,
                             char const* scaleParamsStr, char const* rangeParamsStr, char const* rtpInfoParamsStr);
  Boolean handleTEARDOWNResponse(MediaSession& session, MediaSubsession& subsession);
  Boolean handleGET_PARAMETERResponse(char const* parameterName, char*& resultValueString);
  Boolean handleAuthenticationFailure(char const* wwwAuthenticateParamsStr);
  Boolean resendCommand(RequestRecord* request);
  char const* sessionURL(MediaSession const& session) const;
  static void handleAlternativeRequestByte(void*, u_int8_t requestByte);
  void handleAlternativeRequestByte1(u_int8_t requestByte);
  void constructSubsessionURL(MediaSubsession const& subsession,
			      char const*& prefix,
			      char const*& separator,
			      char const*& suffix);

  // Support for tunneling RTSP-over-HTTP:
  Boolean setupHTTPTunneling1(); // send the HTTP "GET"
  static void responseHandlerForHTTP_GET(RTSPClient* rtspClient, int responseCode, char* responseString);
  void responseHandlerForHTTP_GET1(int responseCode, char* responseString);
  Boolean setupHTTPTunneling2(); // send the HTTP "POST"

  // Support for asynchronous connections to the server:
  static void connectionHandler(void*, int /*mask*/);
  void connectionHandler1();

  // Support for handling data sent back by a server:
  static void incomingDataHandler(void*, int /*mask*/);
  void incomingDataHandler1();
  void handleResponseBytes(int newBytesRead);

private:
  int fVerbosityLevel;
  portNumBits fTunnelOverHTTPPortNum;
  char* fUserAgentHeaderStr;
  unsigned fUserAgentHeaderStrLen;
  int fInputSocketNum, fOutputSocketNum;
  unsigned fServerAddress;
  unsigned fCSeq; // sequence number, used in consecutive requests
  char* fBaseURL;
  Authenticator fCurrentAuthenticator;
  unsigned char fTCPStreamIdCount; // used for (optional) RTP/TCP
  char* fLastSessionId;
  unsigned fSessionTimeoutParameter; // optionally set in response "Session:" headers
  char* fResponseBuffer;
  unsigned fResponseBytesAlreadySeen, fResponseBufferBytesLeft;
  RequestQueue fRequestsAwaitingConnection, fRequestsAwaitingHTTPTunneling, fRequestsAwaitingResponse;

  // Support for tunneling RTSP-over-HTTP:
  char fSessionCookie[33];
  unsigned fSessionCookieCounter;
  Boolean fHTTPTunnelingConnectionIsPending;

#ifdef RTSPCLIENT_SYNCHRONOUS_INTERFACE
  // Old "RTSPClient" interface, which performs synchronous (blocking) operations.
  // This will eventually go away, so new applications should not use it.
public:
  static RTSPClient* createNew(UsageEnvironment& env,
                               int verbosityLevel = 0,
                               char const* applicationName = NULL,
                               portNumBits tunnelOverHTTPPortNum = 0);
  char* describeURL(char const* url, Authenticator* authenticator = NULL,
                    Boolean allowKasennaProtocol = False, int timeout = -1);
  char* describeWithPassword(char const* url,
			     char const* username, char const* password,
			     Boolean allowKasennaProtocol = False, 
			     int timeout = -1);
  char* sendOptionsCmd(char const* url,
		       char* username = NULL, char* password = NULL,
		       Authenticator* authenticator = NULL,
		       int timeout = -1);
  Boolean announceSDPDescription(char const* url,
				 char const* sdpDescription,
				 Authenticator* authenticator = NULL,
				 int timeout = -1);
  Boolean announceWithPassword(char const* url, char const* sdpDescription,
			       char const* username, char const* password, int timeout = -1);
  Boolean setupMediaSubsession(MediaSubsession& subsession,
			       Boolean streamOutgoing = False,
			       Boolean streamUsingTCP = False,
			       Boolean forceMulticastOnUnspecified = False);
  Boolean playMediaSession(MediaSession& session,
			   double start = 0.0f, double end = -1.0f,
			   float scale = 1.0f);
  Boolean playMediaSubsession(MediaSubsession& subsession,
			      double start = 0.0f, double end = -1.0f,
			      float scale = 1.0f,
			      Boolean hackForDSS = False);
  Boolean pauseMediaSession(MediaSession& session);
  Boolean pauseMediaSubsession(MediaSubsession& subsession);
  Boolean recordMediaSubsession(MediaSubsession& subsession);
  Boolean setMediaSessionParameter(MediaSession& session,
				   char const* parameterName,
				   char const* parameterValue);
  Boolean getMediaSessionParameter(MediaSession& session,
				   char const* parameterName,
				   char*& parameterValue);
  Boolean teardownMediaSession(MediaSession& session);
  Boolean teardownMediaSubsession(MediaSubsession& subsession);
private: // used to implement the old interface:
  static void responseHandlerForSyncInterface(RTSPClient* rtspClient,
					      int responseCode, char* responseString);
  void responseHandlerForSyncInterface1(int responseCode, char* responseString);
  static void timeoutHandlerForSyncInterface(void* rtspClient);
  void timeoutHandlerForSyncInterface1();
  TaskToken fTimeoutTask;
  char fWatchVariableForSyncInterface;
  char* fResultString;
  int fResultCode;
#endif
};

#endif

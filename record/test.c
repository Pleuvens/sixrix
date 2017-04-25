1 
9 /*
10  * $Id$
11  *
12  * This program uses the PortAudio Portable Audio Library.
13  * For more information see: http://www.portaudio.com
14  * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
15  *
16  * Permission is hereby granted, free of charge, to any person obtaining
17  * a copy of this software and associated documentation files
18  * (the "Software"), to deal in the Software without restriction,
19  * including without limitation the rights to use, copy, modify, merge,
20  * publish, distribute, sublicense, and/or sell copies of the Software,
21  * and to permit persons to whom the Software is furnished to do so,
22  * subject to the following conditions:
23  *
24  * The above copyright notice and this permission notice shall be
25  * included in all copies or substantial portions of the Software.
26  *
27  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
28  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
29  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
30  * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
31  * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
32  * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
33  * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
34  */
35
36 /*
37  * The text above constitutes the entire PortAudio license; however,
38  * the PortAudio community also makes the following non-binding requests:
39  *
40  * Any person wishing to distribute modifications to the Software is
41  * requested to send the modifications to the original developer so that
42  * they can be incorporated into the canonical version. It is also
43  * requested that these non-binding requests be included along with the
44  * license above.
45  */
46
47 #include <stdio.h>
48 #include <math.h>
49 #include "portaudio.h"
50
51 #ifdef WIN32
52 #include <windows.h>
53
54 #if PA_USE_ASIO
55 #include "pa_asio.h"
56 #endif
57 #endif
58
59 /*******************************************************************/
60 static void PrintSupportedStandardSampleRates(
61         const PaStreamParameters *inputParameters,
62         const PaStreamParameters *outputParameters )
63 {
64     static double standardSampleRates[] = {
65         8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
66         44100.0, 48000.0, 88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
67     };
68     int     i, printCount;
69     PaError err;
70
71     printCount = 0;
72     for( i=0; standardSampleRates[i] > 0; i++ )
73     {
74         err = Pa_IsFormatSupported( inputParameters, outputParameters, standardSampleRates[i] );
75         if( err == paFormatIsSupported )
76         {
77             if( printCount == 0 )
78             {
79                 printf( "\t%8.2f", standardSampleRates[i] );
80                 printCount = 1;
81             }
82             else if( printCount == 4 )
83             {
84                 printf( ",\n\t%8.2f", standardSampleRates[i] );
85                 printCount = 1;
86             }
87             else
88             {
89                 printf( ", %8.2f", standardSampleRates[i] );
90                 ++printCount;
91             }
92         }
93     }
94     if( !printCount )
95         printf( "None\n" );
96     else
97         printf( "\n" );
98 }
99
100 /*******************************************************************/
101 int main(void);
102 int main(void)
103 {
104     int     i, numDevices, defaultDisplayed;
105     const   PaDeviceInfo *deviceInfo;
106     PaStreamParameters inputParameters, outputParameters;
107     PaError err;
108
109
110     err = Pa_Initialize();
111     if( err != paNoError )
112     {
113         printf( "ERROR: Pa_Initialize returned 0x%x\n", err );
114         goto error;
115     }
116
117     printf( "PortAudio version: 0x%08X\n", Pa_GetVersion());
118     printf( "Version text: '%s'\n", Pa_GetVersionInfo()->versionText );
119
120     numDevices = Pa_GetDeviceCount();
121     if( numDevices < 0 )
122     {
123         printf( "ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices );
124         err = numDevices;
125         goto error;
126     }
127
128     printf( "Number of devices = %d\n", numDevices );
129     for( i=0; i<numDevices; i++ )
130     {
131         deviceInfo = Pa_GetDeviceInfo( i );
132         printf( "--------------------------------------- device #%d\n", i );
133
134     /* Mark global and API specific default devices */
135         defaultDisplayed = 0;
136         if( i == Pa_GetDefaultInputDevice() )
137         {
138             printf( "[ Default Input" );
139             defaultDisplayed = 1;
140         }
141         else if( i == Pa_GetHostApiInfo( deviceInfo->hostApi )->defaultInputDevice )
142         {
143             const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
144             printf( "[ Default %s Input", hostInfo->name );
145             defaultDisplayed = 1;
146         }
147
148         if( i == Pa_GetDefaultOutputDevice() )
149         {
150             printf( (defaultDisplayed ? "," : "[") );
151             printf( " Default Output" );
152             defaultDisplayed = 1;
153         }
154         else if( i == Pa_GetHostApiInfo( deviceInfo->hostApi )->defaultOutputDevice )
155         {
156             const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
157             printf( (defaultDisplayed ? "," : "[") );
158             printf( " Default %s Output", hostInfo->name );
159             defaultDisplayed = 1;
160         }
161
162         if( defaultDisplayed )
163             printf( " ]\n" );
164
165     /* print device info fields */
166 #ifdef WIN32
167         {   /* Use wide char on windows, so we can show UTF-8 encoded device names */
168             wchar_t wideName[MAX_PATH];
169             MultiByteToWideChar(CP_UTF8, 0, deviceInfo->name, -1, wideName, MAX_PATH-1);
170             wprintf( L"Name                        = %s\n", wideName );
171         }
172 #else
173         printf( "Name                        = %s\n", deviceInfo->name );
174 #endif
175         printf( "Host API                    = %s\n",  Pa_GetHostApiInfo( deviceInfo->hostApi )->name );
176         printf( "Max inputs = %d", deviceInfo->maxInputChannels  );
177         printf( ", Max outputs = %d\n", deviceInfo->maxOutputChannels  );
178
179         printf( "Default low input latency   = %8.4f\n", deviceInfo->defaultLowInputLatency  );
180         printf( "Default low output latency  = %8.4f\n", deviceInfo->defaultLowOutputLatency  );
181         printf( "Default high input latency  = %8.4f\n", deviceInfo->defaultHighInputLatency  );
182         printf( "Default high output latency = %8.4f\n", deviceInfo->defaultHighOutputLatency  );
183
184 #ifdef WIN32
185 #if PA_USE_ASIO
186 /* ASIO specific latency information */
187         if( Pa_GetHostApiInfo( deviceInfo->hostApi )->type == paASIO ){
188             long minLatency, maxLatency, preferredLatency, granularity;
189
190             err = PaAsio_GetAvailableLatencyValues( i,
191                             &minLatency, &maxLatency, &preferredLatency, &granularity );
192
193             printf( "ASIO minimum buffer size    = %ld\n", minLatency  );
194             printf( "ASIO maximum buffer size    = %ld\n", maxLatency  );
195             printf( "ASIO preferred buffer size  = %ld\n", preferredLatency  );
196
197             if( granularity == -1 )
198                 printf( "ASIO buffer granularity     = power of 2\n" );
199             else
200                 printf( "ASIO buffer granularity     = %ld\n", granularity  );
201         }
202 #endif /* PA_USE_ASIO */
203 #endif /* WIN32 */
204
205         printf( "Default sample rate         = %8.2f\n", deviceInfo->defaultSampleRate );
206
207     /* poll for standard sample rates */
208         inputParameters.device = i;
209         inputParameters.channelCount = deviceInfo->maxInputChannels;
210         inputParameters.sampleFormat = paInt16;
211         inputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
212         inputParameters.hostApiSpecificStreamInfo = NULL;
213
214         outputParameters.device = i;
215         outputParameters.channelCount = deviceInfo->maxOutputChannels;
216         outputParameters.sampleFormat = paInt16;
217         outputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
218         outputParameters.hostApiSpecificStreamInfo = NULL;
219
220         if( inputParameters.channelCount > 0 )
221         {
222             printf("Supported standard sample rates\n for half-duplex 16 bit %d channel input = \n",
223                     inputParameters.channelCount );
224             PrintSupportedStandardSampleRates( &inputParameters, NULL );
225         }
226
227         if( outputParameters.channelCount > 0 )
228         {
229             printf("Supported standard sample rates\n for half-duplex 16 bit %d channel output = \n",
230                     outputParameters.channelCount );
231             PrintSupportedStandardSampleRates( NULL, &outputParameters );
232         }
233
234         if( inputParameters.channelCount > 0 && outputParameters.channelCount > 0 )
235         {
236             printf("Supported standard sample rates\n for full-duplex 16 bit %d channel input, %d channel output = \n",
237                     inputParameters.channelCount, outputParameters.channelCount );
238             PrintSupportedStandardSampleRates( &inputParameters, &outputParameters );
239         }
240     }
241
242     Pa_Terminate();
243
244     printf("----------------------------------------------\n");
245     return 0;
246
247 error:
248     Pa_Terminate();
249     fprintf( stderr, "Error number: %d\n", err );
250     fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
251     return err;
252 }

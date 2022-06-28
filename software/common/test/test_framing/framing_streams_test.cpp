/* Copyright 2020-2022, RespiraWorks
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "framing_streams.h"

#include "gtest/gtest.h"
#include "observer_stream.h"

TEST(StreamResponse, StreamResponseSum) {
  StreamResponse r1{42, ResponseFlags::StreamSuccess};
  StreamResponse r2{52, ResponseFlags::StreamSuccess};
  r2 += r1;
  ASSERT_EQ(r2.Count(), 42 + 52);
}

TEST(FramingStreamsTests, CrcStreamTrivial) {
  ObserverStream observer;
  CrcStream crc_stream(observer);

  crc_stream.put(0);
  ASSERT_EQ(observer.get_index(), 1);
  ASSERT_EQ(crc_stream.put(EndOfStream).Count(), 4);
  ASSERT_EQ(observer.get_index(), 5);
}

TEST(FramingStreamsTests, EscapeStreamTrivial) {
  ObserverStream observer;
  EscapeStream escape_stream(observer);
  EXPECT_EQ(escape_stream.put(EndOfStream).Count(), 2);
  EXPECT_EQ(escape_stream.put(0).Count(), 2);
  EXPECT_EQ(escape_stream.put(0).Count(), 1);
  EXPECT_EQ(escape_stream.put(FramingMark).Count(), 2);
  EXPECT_EQ(escape_stream.put(FramingEscape).Count(), 2);
  EXPECT_EQ(escape_stream.put(EndOfStream).Count(), 1);
}

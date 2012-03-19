/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

namespace GeoGen{
    namespace Net {
        public enum class MessageLevel{
            Message = 0,
            Notice = 1,
            Warning = 2,
            Error = 3
        };

        public ref class MessageEventArgs sealed: System::EventArgs{
        private:
            System::String^ message;
            MessageLevel level;
            int line;
            int column;
        public:
            property System::String^ Message{
                System::String^ get(){
                    return message;
                }
            }

            property MessageLevel Level{
                MessageLevel get(){
                    return level;
                }
            }

            property int Line{
                int get(){
                    return line;
                }
            }

            property int Column{
                int get(){
                    return column;
                }
            }

            MessageEventArgs(System::String^ message, MessageLevel level, int line, int column):
                message(message),
                level(level),
                line(line),
                column(column)
            {}
        };

        public ref class ProgressEventArgs sealed: System::EventArgs{
        private:
            int currentProgress;
            int maximumProgress;
        public:
            property int CurrentProgress{
                int get(){
                    return currentProgress;
                }
            }

            property int MaximumProgress{
                int get(){
                    return maximumProgress;
                }
            }

            ProgressEventArgs(int currentProgress, int maximumProgress):currentProgress(currentProgress),maximumProgress(maximumProgress){}
        };

        public ref class MapReturnedEventArgs sealed: System::EventArgs{
        private:
            System::String^ label;
            HeightData^ heightMap;
        public:
            property System::String^ Label{
                System::String^ get(){
                    return label;
                }
            }

            property HeightData^ HeightMap{
                HeightData^ get(){
                    return heightMap;
                }
            }

            MapReturnedEventArgs(System::String^ label, HeightData^ heightMap): label(label), heightMap(heightMap){}
        };
    }
}
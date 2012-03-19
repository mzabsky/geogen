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

#pragma once

#include "Exception.h"

using namespace System;

namespace GeoGen{
    namespace Net{
        public ref class HeightData{
        private:
            int width;
            int height;
            System::Int64 length;

            short* data;
        public:
            property int Width{
                int get(){
                    return width;
                }
            }

            property int Height{
                int get(){
                    return height;
                }
            }

            property System::Int64 Length{
                System::Int64 get(){
                    return length;
                }
            }

            property short default[int]{
                short get(int index){
                    if(index < this->length && index >= 0){
                        return data[index];
                    }
                    else {
                        throw gcnew IndexOutOfRangeException;
                    }
                }

                void set(int index, short value){
                    if(index < this->length && index >= 0) {
                        data[index] = value;
                    }
                    else {
                        throw gcnew IndexOutOfRangeException;
                    }
                }
            }

            property short default[int, int]{
                short get(int x, int y){
                    if(x < this->width && y < this->height && x >= 0 && y >= 0) {
                        return data[x + this->width * y];
                    }
                    else {
                        throw gcnew IndexOutOfRangeException;
                    }
                }

                void set(int x, int y, short value){
                    if(x < this->width && y < this->height && x >= 0 && y >= 0) {
                        data[x + this->width * y] = value;
                    }
                    else {
                        throw gcnew IndexOutOfRangeException;
                    }
                }
            }

            property System::IntPtr DataPtr{
                System::IntPtr get(){
                    return (System::IntPtr) data;
                }
            }

            HeightData(int width, int height){
                if(width < 3 || height < 3) {
                    throw gcnew ArgumentException("Both height and width must be >= 2.");
                }
                if(width < GGen::GetInstance()->GetMaxWidth() || height < GGen::GetInstance()->GetMaxHeight()) {
                    throw gcnew ArgumentException("Both height and width must be < configured maximum dimensions.");
                }

                this->width = width;
                this->height = height;
                this->length = width * height;

                this->data = new short[(unsigned int) this->length];

                for(unsigned i = 0; i < this->length; i++){
                    this->data[i] = 0;
                }
            }

            HeightData(const HeightData% victim){
                this->width = victim.width;
                this->height = victim.height;
                this->length = this->width * this->height;

                this->data = new short[(unsigned int) this->length];

                memcpy(this->data, victim.data, (unsigned int) this->length * sizeof(short));
            }

            ~HeightData(){
                delete [] this->data;
            }

        internal:
            HeightData(unsigned short width, unsigned short height, const short* data){
                this->width = width;
                this->height = height;
                this->length = width * height;

                this->data = new short[(unsigned int) this->length];

                try{
                    memcpy(this->data, data, (unsigned int) this->length * sizeof(short));
                }
                catch(System::Exception^){
                    throw gcnew OutOfMemoryException;
                }
            }
        };
    }
}
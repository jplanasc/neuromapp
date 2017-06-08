/*this is the compression policy file to be used by the block*/
#ifndef neuromapp_COMPRESSOR
#define neuromapp_COMPRESSOR

#include <cstdlib>
#include <stdexcept> 
#include <cstdlib>
#include "zlib.h"
#include "block.h"
//TODO change back from std namespace 
using namespace std;
namespace neuromapp {
    class no_compress {
        public:
            void compress_policy(void * data_source, size_t uncompressed_size ){ 
                std::cout << "compression library not found, can't compress, leaving the same size as before" << std::endl;
            }

            void uncompress_policy(void * data_source,size_t compressed_size,size_t uncompressed_size) {
                std::cout << "no compression library found" << std::endl;
            }
    };

    class zlib {
        public:
            //double pointer so we can change the upper_level within the function
            template<typename data_type>
            void compress_policy(data_type ** data_source, size_t *uncompressed_size) {
                //get the approximate size in memory of the data_source
                uLong source_len = (uLong) *uncompressed_size;
                uLong dest_len = compressBound(source_len);
                //create void buffer holder dest for the compressed block
                data_type * dest = (data_type *) malloc(dest_len);// creates enough space for the compressed block
                // assign Bytef* for source and dest
                Bytef* source_ptr = (Bytef*) *data_source;// hopefully typedef pointer (data() return) can be coerced to Bytef*
                Bytef* dest_ptr = (Bytef*) dest;
                //perform the actual compression
                int rc = compress(dest_ptr,&dest_len,source_ptr,source_len);
                //check the rc to evaluate compress success
                switch (rc) {
                    case Z_OK:
                        std::cout << "compress worked" << std::endl;
                        break;
                    case Z_BUF_ERROR:
                        std::cerr << "ran out of space in compress buffer"<< std::endl;
                        throw runtime_error("");
                        break;
                    case Z_MEM_ERROR:
                        std::cerr << "ran out of memory during compression"<< std::endl;
                        throw runtime_error("");
                        // do we still need the break underneath this?
                        break;
                }
                //update the size of the block
                *uncompressed_size = dest_len;
                // change the uncompressed data out for compressed
                *data_source = dest;
            }


            //double pointer so we can change the upper_level within the function
            template<typename data_type>
            void uncompress_policy(data_type ** data_source, size_t *compressed_size, size_t uncompressed_size) {
                //original amount of memory used is still discernable
                uLong dest_len = (uLong) uncompressed_size;
                data_type * dest = (data_type *) std::malloc(dest_len);
                //need to get the compressed source size to work with
                uLong source_len = *compressed_size;
                //set pointers for uncompress
                Bytef* source_ptr = (Bytef*) *data_source;
                Bytef* dest_ptr = (Bytef*) dest;
                //perform the uncompress
                int rc = uncompress(dest_ptr,&dest_len,source_ptr,source_len);
                switch (rc) {
                    case Z_OK:
                        std::cout << "compress worked" << std::endl;
                        break;
                    case Z_BUF_ERROR:
                        throw zlib_exception("ran out of space in buffer",1);
                        break;
                    case Z_MEM_ERROR:
                        throw zlib_exception("ran out of memory",2);
                        // do we still need the break underneath this?
                        break;
                }
                //swap the data
                *compressed_size = uncompressed_size;
                *data_source = dest;
            }
    };

    //end of neuromapp namespace
}

#endif




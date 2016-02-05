#ifndef NEUROMAPP_PATH
#define NEUROMAPP_PATH

namespace mapp{
     /** Helper to obtain the path in the build directory

     Data are saved under .zip/.tar format, they are unzip/untar and put in the build directory
     */
     struct helper_build_path{
         const std::string static test_data_path(){
             return "/Users/ewart/Desktop/LICENCE/neuromapp/bc/test/";
         }
     };
}
#endif

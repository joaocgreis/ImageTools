extern "C" {
#include <ppm.h>
}

#include <iostream>
#include <sstream>
using namespace std;

#define MINW 240

int minw, minh;

pixel** src;
int scols, srows;
pixval smaxval;
pixel** dst;
int dcols, drows;
pixval dmaxval;
pixel** img;
int cols, rows;
pixval maxval;

void work ()
{
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (PPM_EQUAL (img[r][c], src[r][c])) {
        img[r][c] = dst[r][c];
      }
    }
  }
}

int main (int argc, char** argv)
{
  minw = minh = 50;
  
  {
    FILE* fp;
    fp = fopen (argv[1], "rb");
    src = ppm_readppm (fp, &scols, &srows, &smaxval);
    fclose (fp);
  }
  
  {
    FILE* fp;
    fp = fopen (argv[2], "rb");
    dst = ppm_readppm (fp, &dcols, &drows, &dmaxval);
    fclose (fp);
  }
  
  for (int i = 3; i < argc; i++) {
    cout << argv[i];
    
    {
      FILE* fp;
      fp = fopen (argv[i], "rb");
      img = ppm_readppm (fp, &cols, &rows, &maxval);
      fclose (fp);
    }
    
    work ();
    
    {
      FILE* fp;
      ostringstream s;
      s << "mod-";
      s << argv[i];
      fp = fopen (s.str().c_str(), "wb");
      ppm_writeppm (fp, img, cols, rows, maxval, 0);
      fclose (fp);
    }
    
    ppm_freearray (img, rows);
    
    cout << endl;
  }
  
  ppm_freearray (src, srows);
  ppm_freearray (dst, drows);
  
  return 0;
}

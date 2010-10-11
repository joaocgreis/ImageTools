// Fazer ps
// pnmtops img-0000.ppm > a.ps

// Rodar pdf
// gs -sDEVICE=pdfwrite -sOutputFile="output.pdf" -dNOPAUSE -dEPSCrop -c "<</Orientation 3>> setpagedevice" -f "a.ps" -c quit

extern "C" {
#include <ppm.h>
}

#include <iostream>
#include <sstream>
using namespace std;

#define MINW 240

int minw, minh;

void rect (pixel** img, pixval maxval, int cmin, int cmax, int rmin, int rmax)
{
  static int count = -1;
  int w = cmax - cmin;
  int h = rmax - rmin;
  if ( (w < minw) || (h < minh)) {
    return;
  }
  
  count++;
  
  pixel** newimg = ppm_allocarray (w, h);
  
  for (int r = 0; r < h; r++) {
    for (int c = 0; c < w; c++) {
      newimg[r][c] = img[rmin+r][cmin+c];
    }
  }
  
  {
    FILE* fp;
    ostringstream s;
    s << "img-";
    s.fill ('0');
    s.width (4);
    s << count << ".ppm";
    cout << "  ->" << s.str();
    fp = fopen (s.str().c_str(), "wb");
    ppm_writeppm (fp, newimg, w, h, maxval, 0);
    fclose (fp);
  }
  
  ppm_freearray (newimg, h);
}

void work (pixel** img, int cols, int rows, pixval maxval)
{
  bool in = false;
  int bcmin, bcmax, brmin, brmax;
  for (int r = 0; r < rows; r++) {
    int cmin = 0, cmax = cols - 1;
    for (; cmin < cols; cmin++) {
      if ( (PPM_GETR (img[r][cmin]) < MINW)
           || (PPM_GETG (img[r][cmin]) < MINW)
           || (PPM_GETB (img[r][cmin]) < MINW)) {
        break;
      }
    }
    if (cmin != cols) {
      for (; cmax > 0; cmax--) {
        if ( (PPM_GETR (img[r][cmax]) < MINW)
             || (PPM_GETG (img[r][cmax]) < MINW)
             || (PPM_GETB (img[r][cmax]) < MINW)) {
          break;
        }
      }
      cmax++; // Ends after

      // At this point there is content, and we know min and max.
      if (in) {
        // Keep in
        bcmin = min (bcmin, cmin);
        bcmax = max (bcmax, cmax);
      }
      else {
        // Enter
        in = true;
        bcmin = cmin;
        bcmax = cmax;
        brmin = r;
      }
    }
    else {
      // At this point there is no content.
      if (in) {
        // Exit
        in = false;
        brmax = r;
        rect (img, maxval, bcmin, bcmax, brmin, brmax);
      }
      else {
        // Keep out
      }
    }
  }
  if (in) {
    // Last exit
    brmax = rows;
    rect (img, maxval, bcmin, bcmax, brmin, brmax);
  }
}

int main (int argc, char** argv)
{
  minw = minh = 50;
  
  pixel** img;
  int cols, rows;
  pixval maxval;
  
  for (int i = 1; i < argc; i++) {
    cout << argv[i];
    
    {
      FILE* fp;
      fp = fopen (argv[i], "rb");
      img = ppm_readppm (fp, &cols, &rows, &maxval);
      fclose (fp);
    }
    
    work (img, cols, rows, maxval);
    
    ppm_freearray (img, rows);
    
    cout << endl;
  }
  
  return 0;
}

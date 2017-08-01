/*
 * MIT License
 *
 * Copyright (c) 2017 Lucas Abel <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "u/fs.h"

int64_t ufsize(ustr_t path) {
#if COMPILER_MSVC
  WIN32_FILE_ATTRIBUTE_DATA   fileInfo;
  if (GetFileAttributesExA(path, GetFileExInfoStandard, (void*)&fileInfo) == 0) return -1;
  return (uint64_t)(((__int64)fileInfo.nFileSizeHigh) << 32 ) + fileInfo.nFileSizeLow;
#else
  struct stat sb;
  if (stat(path, &sb) > 0)
    return -1;
  return (int64_t) sb.st_size;
#endif
}

bool ufexists(ustr_t path) {
#if COMPILER_MSVC
  BOOL isDirectory;
  DWORD attributes = GetFileAttributesA(path);

  // special directory case to drive the network path check
  if (attributes == INVALID_FILE_ATTRIBUTES)
    isDirectory = (GetLastError() == ERROR_BAD_NETPATH);
  else
    isDirectory = (FILE_ATTRIBUTE_DIRECTORY & attributes);

  if (isDirectory) {
    if (PathIsNetworkPathA(path)) return true;
    if (PathIsUNCA(path)) return true;
  }

  if (PathFileExistsA(path) == 1) return true;
#else
  if (access(path, F_OK) == 0)
    return true;
#endif
  return false;
}

ustr_t ufread(ustr_t path) {
  int fd = 0;
  off_t fsize = 0;
  ssize_t fsize2 = 0;
  char *buffer = NULL;

  fsize = (size_t) ufsize(path);
  if (fsize < 0)
    goto abort_read;
  fd = open(path, O_RDONLY);
  if (fd < 0)
    goto abort_read;
  buffer = (char *) malloc((size_t) fsize + 1);
  if (buffer == NULL)
    goto abort_read;
  buffer[fsize] = 0;
  fsize2 = read(fd, buffer, (size_t) fsize);
  if (fsize2 == -1)
    goto abort_read;
  close(fd);
  return ustrn(buffer, (size_t) fsize2);
    abort_read:
  if (buffer)
    free((void *) buffer);
  if (fd >= 0)
    close(fd);
  return NULL;
}

bool ufwrite(ustr_t path, ustr_t buffer) {
#if PLATFORM_WINDOWS
  unsigned mode = _S_IWRITE;
#else
  unsigned mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
#endif
  int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
  size_t len = ustrlen(buffer);

  if (fd < 0)
    return false;
  ssize_t nwrite = write(fd, buffer, len);
  close(fd);
  return ((size_t) nwrite == len);
}

/*
 * Copyright (c) 2008 Remko Troncon.
 */

#ifndef COCOAINITIALIZER_H
#define COCOAINITIALIZER_H

class CocoaInitializer
{
 public:
  CocoaInitializer(void);
  ~CocoaInitializer();

 private:
  class Private;
  Private *d;
};

#endif

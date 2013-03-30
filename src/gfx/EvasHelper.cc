/*
 * --- GSMP-COPYRIGHT-NOTE-BEGIN ---
 * 
 * This copyright note is auto-generated by ./scripts/Create-CopyPatch.
 * Please add additional copyright information _after_ the line containing
 * the GSMP-COPYRIGHT-NOTE-END tag. Otherwise it might get removed by
 * the ./scripts/Create-CopyPatch script. Do not edit this copyright text!
 * 
 * GSMP: gfx/src/EvasHelper.cc
 * General Sound Manipulation Program is Copyright (C) 2000 - 2007
 *   Valentin Ziegler and René Rebe
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2. A copy of the GNU General
 * Public License can be found in the file LICENSE.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANT-
 * ABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * --- GSMP-COPYRIGHT-NOTE-END ---
 */

#include <iostream>

#include "EvasHelper.hh"

EvasCanvas::EvasCanvas () {
  evas_init ();
  ob = evas_new ();
}

EvasCanvas::~EvasCanvas () {
  evas_free (ob);
}

bool EvasCanvas::OutputMethod (const std::string& method)
{
  int renderer = evas_render_method_lookup (method.c_str () );
  if (renderer > 0) {
    evas_output_method_set (ob, renderer);
    return true;
  }
  std::cout << "Error: failed to set output method!" << std::endl;
  return false;
}

Evas_Engine_Info* EvasCanvas::EngineInfo ()
{
  Evas_Engine_Info* info = evas_engine_info_get (ob);
  if (!info)
    std::cout << "Warning: Obtained Evas engine info is INVALID!" << std::endl;
  return info;
};


// ---

EvasBase::EvasBase (bool i_owner)
  : owner (i_owner)
{
}

EvasBase::~EvasBase ()
{
  if (owner) {
    evas_object_del (ob);
  }
}

// ---

EvasImage::EvasImage (EvasCanvas& evas)
  : EvasBase (true)
{
  ob = evas_object_image_add (evas.c_obj () );
}

EvasImage::EvasImage (Evas_Object* i_ob)
  : EvasBase (false)
{
  ob = i_ob;
}

EvasImage::~EvasImage ()
{
}

bool EvasImage::SmartLoad (const std::string& fname)
{
  evas_object_image_file_set(ob, fname.c_str (), NULL);
  
  int err = evas_object_image_load_error_get (ob);
  if (err) {
    std::cout << "Error: loading image: " << err << std::endl;
    return false;
  }
  
  evas_object_image_size_get (ob, &cached_w, &cached_h);
  
  // Yeah - don't ask why Evas doesn't do this ... ?!?
  evas_object_image_fill_set (ob, 0, 0, cached_w, cached_h);
  evas_object_resize (ob, cached_w, cached_h);
  return true;
}


void EvasImage::ApplyAlpha (uint8_t alpha)
{
  int iw ,ih;
  evas_object_image_size_get (ob, &iw, &ih);
  evas_object_image_fill_set (ob, 0, 0, iw, ih);
  uint8_t* data = (uint8_t*) evas_object_image_data_get (ob, 1);
  uint8_t* data_ptr = data;
  uint8_t* data_end = data + (iw * ih * 4);

  while (data_ptr < data_end) {
    *data_ptr = (*data_ptr * alpha) >> 8; data_ptr++;
    *data_ptr = (*data_ptr * alpha) >> 8; data_ptr++;
    *data_ptr = (*data_ptr * alpha) >> 8; data_ptr++;
    *data_ptr = (*data_ptr * alpha) >> 8; data_ptr++;
  }
  
  evas_object_image_data_update_add (ob, 0, 0, iw, ih);
  evas_object_image_data_set (ob, data);
}

// ---

EvasText::EvasText (EvasCanvas& evas)
  : EvasBase (true)
{
  ob = evas_object_text_add (evas.c_obj () );
}


EvasText::EvasText (Evas_Object* i_ob)
  : EvasBase (false)
{
  ob = i_ob;
}

EvasText::~EvasText ()
{
}

// ...

EvasRectangle::EvasRectangle (EvasCanvas& evas)
  : EvasBase (true)
{
  ob = evas_object_rectangle_add (evas.c_obj () );
}


EvasRectangle::EvasRectangle (Evas_Object* i_ob)
  : EvasBase (false)
{
  ob = i_ob;
}

EvasRectangle::~EvasRectangle ()
{
}
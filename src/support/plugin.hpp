/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

#ifndef _NANOS_PLUGIN
#define _NANOS_PLUGIN

#include <string>
#include <vector>
#include "config.hpp"

namespace nanos
{

   class Plugin
   {

      private:
         std::string    _name;
         int            _version;
         void  *        _handler;

         Plugin ( const Plugin & );
         const Plugin operator= ( const Plugin & );
         
      public:
         Plugin( std::string &_name, int _version ) : _name( _name ),_version( _version ) {}

         Plugin( const char *_name, int _version ) : _name( _name ),_version( _version ) {}

         virtual ~Plugin() {}

         virtual void config(Config &cfg) {}
         virtual void init() {}

         virtual void fini() {}

         const std::string & getName() const { return _name; }

         int getVersion() const { return _version; }
   };

   class PluginManager
   {
      private:
         typedef std::vector<Plugin *>    PluginList;
         static PluginList                _activePlugins;

      public:

         static void init();

         static bool isPlugin ( const char *name );
         static bool isPlugin ( const std::string &name ) { return isPlugin( name.c_str() ); }

         static bool load ( const char *plugin_name, const bool init=true );
         static bool load ( const std::string &plugin_name, const bool init=true ) { return load( plugin_name.c_str(), init ); };
         static Plugin* loadAndGetPlugin ( const char *plugin_name, const bool init=true );
         static Plugin* loadAndGetPlugin ( const std::string &plugin_name, const bool init=true ) { return loadAndGetPlugin( plugin_name.c_str(), init ); }

   };

}

#endif

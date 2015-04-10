# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

from waflib import Logs, Utils, Context
import os

VERSION = "0.3.0"
APPNAME = "bgpparser"
PACKAGE_BUGREPORT = "https://github.com/cawka/bgpparser/issues/"
PACKAGE_URL = "https://github.com/cawka/bgpparser/"
GIT_TAG_PREFIX = "bgpparser-"

def options(opt):
    opt.load(['compiler_cxx', 'compiler_c', 'gnu_dirs'])
    opt.load(['default-compiler-flags', 'pch',
              'boost'],
             tooldir=['.waf-tools'])

def configure(conf):
    conf.load(['compiler_cxx', 'compiler_c', 'gnu_dirs',
               'default-compiler-flags', 'pch',
               'boost'])

    conf.check_cfg(package='liblog4cxx', args=['--cflags', '--libs'], global_define=True,
                   uselib_store='LOG4CXX', mandatory=True)

    conf.check_cfg(package='libxml-2.0', args=['--cflags', '--libs'], global_define=True,
                   uselib_store='LIBXML', mandatory=True)

    conf.check_cxx(lib='pthread', uselib_store='PTHREAD', define_name='HAVE_PTHREAD', mandatory=False)
    conf.check_cxx(lib='rt', uselib_store='RT', define_name='HAVE_RT', mandatory=False)
    conf.check_cxx(cxxflags=['-fPIC'], uselib_store='PIC', mandatory=False)

    USED_BOOST_LIBS = ['system', 'filesystem', 'program_options', 'regex', 'iostreams']
    conf.check_boost(lib=USED_BOOST_LIBS, mandatory=True)
    if conf.env.BOOST_VERSION_NUMBER < 104400:
        Logs.error("Minimum required boost version is 1.44.0")
        Logs.error("Please upgrade your distribution or install custom boost libraries" +
                    " (http://redmine.named-data.net/projects/nfd/wiki/Boost_FAQ)")
        return

    conf.write_config_header('include/bgpparser-config.hpp', define_prefix='BGPPARSER_')

def build(bld):
    version(bld)

    bld(features="subst",
        name='version',
        source='include/version.hpp.in',
        target='include/version.hpp',
        install_path=None,
        VERSION_STRING=VERSION_BASE,
        VERSION_BUILD=VERSION,
        VERSION=int(VERSION_SPLIT[0]) * 1000000 +
                int(VERSION_SPLIT[1]) * 1000 +
                int(VERSION_SPLIT[2]),
        VERSION_MAJOR=VERSION_SPLIT[0],
        VERSION_MINOR=VERSION_SPLIT[1],
        VERSION_PATCH=VERSION_SPLIT[2],
        )

    headers = bld.path.ant_glob(['include/**/*.h'])
    bld.install_files("%s/bgpparser" % bld.env['INCLUDEDIR'], headers,
                      relative_trick=True, cwd=bld.path.find_node('include'))

    bld.install_files("%s/bgpparser" % bld.env['INCLUDEDIR'],
                      bld.path.find_resource('include/bgpparser-config.hpp'))

    bld.install_files("%s/bgpparser" % bld.env['INCLUDEDIR'],
                      bld.path.find_resource('include/version.hpp'))

    bld.recurse("parser")
    bld.recurse(["dumper", "dumper2"])
    
def version(ctx):
    if getattr(Context.g_module, 'VERSION_BASE', None):
        return

    Context.g_module.VERSION_BASE = Context.g_module.VERSION
    Context.g_module.VERSION_SPLIT = [v for v in VERSION_BASE.split('.')]

    didGetVersion = False
    try:
        cmd = ['git', 'describe', '--always', '--match', '%s*' % GIT_TAG_PREFIX]
        p = Utils.subprocess.Popen(cmd, stdout=Utils.subprocess.PIPE,
                                   stderr=None, stdin=None)
        out = str(p.communicate()[0].strip())
        didGetVersion = (p.returncode == 0 and out != "")
        if didGetVersion:
            if out.startswith(GIT_TAG_PREFIX):
                Context.g_module.VERSION = out[len(GIT_TAG_PREFIX):]
            else:
                Context.g_module.VERSION = "%s-commit-%s" % (Context.g_module.VERSION_BASE, out)
    except OSError:
        pass

    versionFile = ctx.path.find_node('VERSION')

    if not didGetVersion and versionFile is not None:
        try:
            Context.g_module.VERSION = versionFile.read()
            return
        except (OSError, IOError):
            pass

    # version was obtained from git, update VERSION file if necessary
    if versionFile is not None:
        try:
            version = versionFile.read()
            if version == Context.g_module.VERSION:
                return # no need to update
        except (OSError, IOError):
            Logs.warn("VERSION file exists, but not readable")
    else:
        versionFile = ctx.path.make_node('VERSION')

    if versionFile is None:
        return

    try:
        versionFile.write(Context.g_module.VERSION)
    except (OSError, IOError):
        Logs.warn("VERSION file is not writeable")

def dist(ctx):
    version(ctx)

def distcheck(ctx):
    version(ctx)

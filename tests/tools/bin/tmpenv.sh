if [ `uname -n | grep rsplus` ]; then
ulimit -d 400000
ulimit -s 100000
ulimit -m 100000
if [ X$REFTREE = Xprod ]; then
echo "Nothing set for TMPDIR!"
else
export TMPDIR=/afs/cern.ch/sw/geant4/stt/tmpdir
fi
fi

if [ `uname -n | grep sunasd1` ]; then
echo "Nothing set!"
fi

if [ `uname -n | grep hpplus` ]; then
echo "Nothing set for limit!"
# there is a pretty useless looking file from april in
# the directory below - I bet we are screwed if anything goes there
# with concurrent tests.
if [ X$REFTREE = Xprod ]; then
export TMPDIR=/afs/cern.ch/sw/geant4/stt/tmpdir
else
export TMPDIR=/afs/cern.ch/sw/geant4/stt/tmpdir
fi
fi

if [ `uname -n | grep axcnsi` ]; then
ulimit -d 200000
ulimit -s 20000
echo "Nothing set for TMPDIR!"
fi

if [ `uname -n | grep sgmedia` ]; then
ulimit -s 200000
ulimit -m 400000
if [ X$REFTREE = Xprod ]; then
export TMPDIR=/afs/cern.ch/sw/geant4/stt/tmpdir
else
export TMPDIR=/afs/cern.ch/sw/geant4/stt/tmpdir
fi
fi

if [ `uname -n | grep dxplus` ]; then
echo "I don't know about limits here :)...ooops...already know :("
ulimit -d 400000
fi

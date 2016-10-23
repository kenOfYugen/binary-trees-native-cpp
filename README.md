# binary-trees-native-cpp

C++ binary trees benchmark ported from [the benchmark 
game](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=binarytrees&lang=gpp&id=7) 
as a node.js addon.

## Install
`npm install kenOfYugen/binary-trees-native-cpp`

Tested on Arch Linux.

If it doesn't compile on your system, make sure that the dependencies specified in `binding.gyp` are met, and try again.

## Use

```javascript
var binaryTreesCPP = require('binary-trees-native-cpp');

binaryTreesCPP.run(10);
```

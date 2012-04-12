var path = require('path')
  , fs = require('fs')
  , async = require('async')
  , suite = require('suite');

var genome = suite(path.resolve(__dirname, '../genome'), { prefix: path.resolve(__dirname, 't-') })
  , num = [], files = fs.readdirSync(__dirname).length;

for(var i=1; i<files; i+=1) {
  num.push(i);
}

async.forEachSeries(num, function (i, cb) {
  genome.run(i, cb);
}, function (err) {
  if (err) throw err;;
  genome.statsput();
  if (genome.stats.fail > 0) {
    process.exit(1);
  }
});

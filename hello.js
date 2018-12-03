const sg_module = require('./pkg/sudoku-gen');
const board_count = Number(process.argv[2]);

if (isNaN(board_count)) {
  console.error("\nUsage: node ./hello.js [BOARD_COUNT]\n");
  process.exit(1);
}

 sg_module().then(function(sg) {
  for(let i = 0; i < board_count; i++) {
    console.log(
      sg.generateAndFillBoard().serialize()
    );
  }
})


// Game variables
var canvas, ctx;
var player;
var coins = [];
var score = 0;
var gravity = 2;
var isJumping = false;
var backgroundImage;
var backgroundX = 0;
var lives = 3;

var keys = {
  ArrowLeft: false,
  ArrowRight: false
};

function initializeGame() {
  document.getElementById("gameCanvas").style.display = "block";
  init();
}

// Game initialization
function init() {
  canvas = document.getElementById("gameCanvas");
  ctx = canvas.getContext("2d");

  // Set canvas size to 800x600
  canvas.width = 800;
  canvas.height = 600;

  player = {
    x: canvas.width / 2 - 25, // Fixed X value at the bottom
    y: canvas.height - 450, // Fixed Y value at the bottom
    width: 50,
    height: 50,
    color: "blue",
    speed: 15,
    jumpHeight: 50, // Jump height
    jumpCount: 0, // Number of jumps made
    maxJumpCount: 2 // Maximum number of jumps allowed
  };

  // Randomly select a background image
  var randomIndex = Math.floor(Math.random() * 3) + 1;
  var currentBackgroundImage = "images/background" + randomIndex + ".jpg";

  // Load background image
  backgroundImage = new Image();
  backgroundImage.src = currentBackgroundImage;
  backgroundImage.onload = function () {
    // Start game loop
    requestAnimationFrame(update);
  };

  // Event listeners
  document.addEventListener("keydown", handleKeyPress);
  document.addEventListener("keyup", handleKeyRelease);
}

// Generate a new coin box
function generateCoin() {
  var coinBox = {
    x: Math.random() * (canvas.width - 50), // Random X position
    y: -50, // Start above the canvas
    width: 50,
    height: 50,
    color: "brown",
    speed: Math.random() * 3 + 1, // Random speed between 1 and 4
    coinCount: Math.floor(Math.random() * 3) + 1 // Random number of coins inside the box
  };

  coins.push(coinBox);
}

// Update game state
function update() {
  // Clear canvas
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  // Draw background
  ctx.drawImage(backgroundImage, backgroundX, 0, backgroundImage.width, backgroundImage.height, 0, 0, canvas.width, canvas.height);

  // Draw player
  ctx.fillStyle = player.color;
  ctx.fillRect(player.x, player.y, player.width, player.height);

  // Generate coins randomly
  if (Math.random() < 0.1) {
    generateCoin();
  }

  // Draw coins
  for (var i = 0; i < coins.length; i++) {
    var coinBox = coins[i];

    // Update coin box position
    coinBox.y += coinBox.speed;

    // Check collision with player
    if (
      player.x < coinBox.x + coinBox.width &&
      player.x + player.width > coinBox.x &&
      player.y < coinBox.y + coinBox.height &&
      player.y + player.height > coinBox.y
    ) {
      // Collision detected
      coins.splice(i, 1);
      score += coinBox.coinCount; // Increase the score by the number of coins inside the box
    }

    // Remove coin boxes that have fallen off the screen
    if (coinBox.y > canvas.height) {
      coins.splice(i, 1);
    }

    // Draw coin box
    ctx.fillStyle = coinBox.color;
    ctx.fillRect(coinBox.x, coinBox.y, coinBox.width, coinBox.height);

    // Draw coins inside the box
    var coinSize = 10;
    var coinPadding = 5;
    var coinStartX = coinBox.x + coinPadding;
    var coinStartY = coinBox.y + coinPadding;
    for (var j = 0; j < coinBox.coinCount; j++) {
      ctx.fillStyle = "gold";
      ctx.fillRect(
        coinStartX + (coinSize + coinPadding) * j,
        coinStartY,
        coinSize,
        coinSize
      );
    }

    // Display lives and score
    ctx.fillStyle = "black";
    ctx.font = "20px Arial";
    ctx.fillText("Lives: " + lives, 10, 30);
    ctx.fillText("Score: " + score, 10, 60);
  }

  // Apply gravity to the player
  if (player.y + player.height < 450) {
    player.y += gravity;
  } else {
    player.y = 450 - player.height; // Set player's Y value to the floor
  }

  // Reset jump count if the player touches the ground
  if (player.y === 450 - player.height && isJumping) {
    player.jumpCount = 0;
    isJumping = false;
  }

  updatePlayerPosition();

  // Request next frame
  requestAnimationFrame(update);
}

// Handle key press event
function handleKeyPress(e) {
  if (e.key === "ArrowLeft") {
    keys.ArrowLeft = true;
  } else if (e.key === "ArrowRight") {
    keys.ArrowRight = true;
  } else if (e.key === "ArrowUp" && !isJumping) {
    // Jump if not already jumping
    player.jumpCount++;
    if (player.jumpCount <= player.maxJumpCount) {
      isJumping = true;
      player.y -= player.jumpHeight;
    }
  }
}

// Handle key release event
function handleKeyRelease(e) {
  if (e.key === "ArrowLeft") {
    keys.ArrowLeft = false;
  } else if (e.key === "ArrowRight") {
    keys.ArrowRight = false;
  }
}

// Update player position based on arrow key states
function updatePlayerPosition() {
  if (keys.ArrowLeft && player.x > 0) {
    player.x -= player.speed; // Move left
    if (player.x <= player.speed) {
      var scrollDistance = Math.min(player.speed, -backgroundX); // Calculate the scroll distance
      backgroundX += scrollDistance; // Scroll background to the right
    }
  } else if (keys.ArrowRight && player.x + player.width < canvas.width) {
    player.x += player.speed; // Move right
    if (player.x + player.width >= canvas.width - player.speed) {
      var scrollDistance = Math.min(player.speed, backgroundX + canvas.width - backgroundImage.width); // Calculate the scroll distance
      backgroundX -= scrollDistance; // Scroll background to the left
    }
  }
}

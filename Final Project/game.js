// Game variables
var canvas, ctx;
var player;
var coins = [];
var score = 0;
var gravity = 2;
var isJumping = false;
var currentBackgroundImage = "";
var backgroundImage = new Image();
var backgroundImageX = 0;
var maxPlayerX;
var originalBackgroundWidth; // Store the original width of the background image
var backgroundScrollDirection = 0; // 0: No scrolling, -1: Scroll left, 1: Scroll right

var keys = {
  ArrowLeft: false,
  ArrowRight: false
};

// Coin sprite sheet
var coinImage = new Image();
coinImage.src = "images/coin_sprite_sheet.png";
coinImage.addEventListener("load", init); // Wait for the image to load

// Game initialization
function init() {
  canvas = document.getElementById("gameCanvas");
  ctx = canvas.getContext("2d");

  maxPlayerX = canvas.width / 2;

  // Randomly select a background image
  var randomIndex = Math.floor(Math.random() * 3) + 1;
  currentBackgroundImage = "images/background" + randomIndex + ".jpg";

  // Load and adjust the background image
  backgroundImage.src = currentBackgroundImage;
  backgroundImage.onload = function () {
    var aspectRatio = backgroundImage.width / backgroundImage.height;
    var targetWidth = canvas.height * aspectRatio;
    var targetHeight = canvas.height;
    backgroundImage.width = targetWidth;
    backgroundImage.height = targetHeight;

    originalBackgroundWidth = backgroundImage.width; // Store the original width
  };

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

  // Start game loop
  requestAnimationFrame(update);

  // Event listeners
  document.addEventListener("keydown", handleKeyDown);
  document.addEventListener("keyup", handleKeyUp);
}

// Generate random coins
function generateCoins() {
    var coinX;
  
    // Generate coins randomly outside the visible canvas area
    coinX = Math.random() * (canvas.width + 200) - 100;
  
    var newCoin = {
      x: coinX,
      y: -20, // Start above the canvas
      width: 50,
      height: 80,
      spriteWidth: 205, // Width of each frame in the sprite sheet
      spriteHeight: 260, // Height of each frame in the sprite sheet
      image: coinImage,
      frameIndex: 0, // Current frame index
      frameCount: 0, // Frame counter
      speed: Math.random() * 3 + 1 // Random falling speed
    };
  
    coins.push(newCoin);
  }
  
// Update game state
function update() {
    // Clear canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);
  
    // Calculate the background image position
    var backgroundOffsetX = backgroundImageX % originalBackgroundWidth;
  
    // Draw the background image
    ctx.drawImage(
      backgroundImage,
      backgroundOffsetX,
      0,
      originalBackgroundWidth,
      backgroundImage.height
    );
    ctx.drawImage(
      backgroundImage,
      backgroundOffsetX - originalBackgroundWidth,
      0,
      originalBackgroundWidth,
      backgroundImage.height
    );
  
    // Draw player
    ctx.fillStyle = player.color;
    ctx.fillRect(player.x, player.y, player.width, player.height);
  
    // Draw coins
    for (var i = 0; i < coins.length; i++) {
      var coin = coins[i];
  
      // Update coin position
      coin.y += coin.speed;
  
      // Check collision with player
      if (
        player.x < coin.x + coin.width &&
        player.x + player.width > coin.x &&
        player.y < coin.y + coin.height &&
        player.y + player.height > coin.y
      ) {
        // Collision detected
        coins.splice(i, 1);
        score++;
      }
  
      // Remove coins that have fallen off the screen
      if (coin.y > canvas.height) {
        coins.splice(i, 1);
      }
  
      // Draw animated coin
      ctx.drawImage(
        coin.image,
        coin.frameIndex * coin.spriteWidth,
        0,
        coin.spriteWidth,
        coin.image.height,
        coin.x - backgroundImageX, // Adjust X position based on background scrolling
        coin.y,
        coin.width,
        coin.height
      );
  
      // Update frame index
      coin.frameCount++;
      if (coin.frameCount >= 5) {
        coin.frameIndex++;
        if (coin.frameIndex >= coin.image.width / coin.spriteWidth) {
          coin.frameIndex = 0;
        }
        coin.frameCount = 0;
      }
    }
  
    // Display score
    ctx.fillStyle = "black";
    ctx.font = "20px Arial";
    ctx.fillText("Score: " + score, 10, 30);
  
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
  

// Handle keydown event
function handleKeyDown(e) {
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

// Handle keyup event
function handleKeyUp(e) {
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
      var scrollDistance = Math.min(player.speed, -backgroundImageX); // Calculate the scroll distance
      backgroundImageX += scrollDistance; // Scroll background to the right
    }
  } else if (keys.ArrowRight && player.x + player.width < canvas.width) {
    player.x += player.speed; // Move right
    if (player.x >= canvas.width - player.speed - player.width) {
      var scrollDistance = Math.min(
        player.speed,
        backgroundImage.width + backgroundImageX - canvas.width
      ); // Calculate the scroll distance
      backgroundImageX -= scrollDistance; // Scroll background to the left
    }
  } else if (player.x <= 0 && keys.ArrowLeft) {
    var scrollDistance = Math.min(player.speed, -backgroundImageX); // Calculate the scroll distance
    backgroundImageX += scrollDistance; // Scroll background to the right
  } else if (player.x + player.width >= canvas.width && keys.ArrowRight) {
    var scrollDistance = Math.min(
      player.speed,
      backgroundImage.width + backgroundImageX - canvas.width
    ); // Calculate the scroll distance
    backgroundImageX -= scrollDistance; // Scroll background to the left
  }

  // Update maxPlayerX value when player reaches the border
  if (player.x > maxPlayerX) {
    maxPlayerX = player.x;
  } else if (player.x < maxPlayerX - canvas.width) {
    maxPlayerX = player.x + canvas.width;
  }

  // Prevent player from moving beyond the bottom of the canvas
  if (player.y + player.height > canvas.height) {
    player.y = canvas.height - player.height;
  }
}

// Generate a new coin every second
setInterval(generateCoins, 1000);

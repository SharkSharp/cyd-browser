import Fastify from "fastify";
import puppeteer from "puppeteer";

enum TouchAction {
  up = 0,
  down = 1,
  move = 2,
}

interface Touch {
  action: TouchAction;
  x?: number;
  y?: number;
}

const isTouch = (obj: any): obj is Touch => {
  return (
    obj &&
    typeof obj.action === "number" &&
    (!obj.x || typeof obj.x === "number") &&
    (!obj.y || typeof obj.y === "number")
  );
};

const fastify = Fastify({
  logger: false,
});

(async () => {
  const browser = await puppeteer.launch();
  const page = await browser.newPage();
  await page.setViewport({ width: 320, height: 240 });
  await page.goto("https://youtube.com/");
  await page.mouse.click(0, 0);

  fastify.get("/", async function handler(request, reply) {
    const screenshot = await page.screenshot({
      encoding: "binary",
      type: "jpeg",
    });
    reply.send(screenshot);
  });

  fastify.put("/", async function handler(request, reply) {
    if (isTouch(request.body)) {
      const { action, x, y } = request.body;
      switch (action) {
        case TouchAction.up:
          await page.touchscreen.touchEnd();
          break;
        case TouchAction.down:
          await page.touchscreen.touchStart(x!, y!);
          break;
        case TouchAction.move:
          await page.touchscreen.touchMove(x!, y!);
          break;
      }
    }
    reply.send("OK");
  });

  try {
    await fastify.listen({ port: 3000, host: "0.0.0.0" });
  } catch (err) {
    fastify.log.error(err);
    await browser.close();
    process.exit(1);
  }
})();

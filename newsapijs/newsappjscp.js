const express = require("express");
const bodyParser = require("body-parser");
const db = require("./db");


// defining the Express app
const app = express();
    //define the port
const port = 3000;
    // use the express-static middleware
app.use(express.static("public"));
    //create app database if it doesn't exist
db.createDatabase();
//db connection
db.connect();

// create API keys from file 
db.readAPIKeysFromFile();

//apk key authentication using middleware
app.use((req, res, next) => {
  const apiKey = req.headers["api-key"];
  if (!apiKey || !db.getAPIKey(apiKey)) {
    res.status(401).send("Unauthorized");
    return;
  }

  next();
});
// api keys hashing using middleware
app.use((req, res, next) => {
  const apiKey = req.headers["api-key"];
  if (apiKey) {
    req.apiKeyHash = db.hashAPIKey(apiKey);
  }

  next();
});
// Allow users and apps to read and consumer news articles
app.use((req, res, next) => {
  res.setHeader("Access-Control-Allow-Origin", "*");
  next();
});
// create an endpoint for users to read news articles
app.get("/news", async (req, res) => {
  const newsArticles = await db.getAllNewsArticles();
  res.json(newsArticles);
});
// create an endpoint for users to search news articles
app.get("/news/search", async (req, res) => {
  const query = req.query.query;
  const newsArticles = await db.searchNewsArticles(query);
  res.json(newsArticles);
});
// create an endpoint for users to read a single news article
app.get("/news/:id", async (req, res) => {
  const id = req.params.id;
  const newsArticle = await db.getNewsArticleById(id);
  if (!newsArticle) {
    res.status(404).send("News article not found");
    return;
  }

  res.json(newsArticle);
});
// create an endpoint for users to read  breaking news from top news sources
app.get("/news/breaking", async (req, res) => {
  const newsArticles = await db.getBreakingNews();
  res.json(newsArticles);
});
// ceate a POST endpoint for users to create news articles
app.post("/news", bodyParser.json(), async (req, res) => {
  const newsArticle = req.body;
  const id = await db.insertNewsArticle(newsArticle);
  res.json({ id: id });
});
// create a POST endpoint for users to create comments
app.post("/news/:id/comments", bodyParser.json(), async (req, res) => {
  const id = req.params.id;
  const comment = req.body;
  await db.addComment(id, comment);
  res.status(201).send("Comment added");
});
// createa an endpoint for users to delete news article whihc they posted only
// make a check if user is authorized to delete the news article
app.delete("/news/:id", async (req, res) => {
  const id = req.params.id;
  const newsArticle = await db.getNewsArticleById(id);
  if (!newsArticle) {
    res.status(404).send("News article not found");
    return;
  }
  //allow only the user who created the news article to delete it


  if (newsArticle.apiKeyHash !== req.apiKeyHash) {
    res.status(403).send("Forbidden");
    return;
  }

  await db.deleteNewsArticle(id);
  res.status(204).send();
});
// create an endpoint for users to edit news articles
app.put("/news/:id", bodyParser.json(), async (req, res) => {
  const id = req.params.id;
  const newsArticle = await db.getNewsArticleById(id);
  if (!newsArticle) {
    res.status(404).send("News article not found");
    return;
  }
  //allow users to seearch news articles by category
    //allow only the user who created the news article to edit it
    if (newsArticle.apiKeyHash !== req.apiKeyHash) {
        res.status(403).send("Forbidden");
        return;
        }
    //allow users to edit news articles by category
    const updatedNewsArticle = req.body;
    await db.updateNewsArticle(id, updatedNewsArticle);
    res.status(200).send("News article updated");
}
);
// start the Express server
app.listen(port, () => {
  console.log(`Server running on port ${port}`);
});



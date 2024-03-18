const express = require("express");
const bodyParser = require("body-parser");
const db = require("./db");

const app = express();

// API key authentication
app.use((req, res, next) => {
  const apiKey = req.headers["api-key"];
  if (!apiKey || !db.getAPIKey(apiKey)) {
    res.status(401).send("Unauthorized");
    return;
  }

  next();
});

// GET /news
app.get("/news", async (req, res) => {
  const newsArticles = await db.getAllNewsArticles();
  res.json(newsArticles);
});

// GET /news/search
app.get("/news/search", async (req, res) => {
  const query = req.query.query;
  const newsArticles = await db.searchNewsArticles(query);
  res.json(newsArticles);
});

// GET /news/:id
app.get("/news/:id", async (req, res) => {
  const id = req.params.id;
  const newsArticle = await db.getNewsArticleById(id);
  if (!newsArticle) {
    res.status(404).send("News article not found");
    return;
  }

  res.json(newsArticle);
});

// POST /news
app.post("/news", async (req, res) => {
  const newsArticle = req.body;
  await db.addNewsArticle(newsArticle);
  res.status(201).send("News article created");
});

// PUT /news/:id
app.put("/news/:id", async (req, res) => {
  const id = req.params.id;
  const newsArticle = req.body;
  await db.updateNewsArticle(id, newsArticle);
  res.status(200).send("News article updated");
});

// DELETE /news/:id
app.delete("/news/:id", async (req, res) => {
  const id = req.params.id;
  await db.deleteNewsArticle(id);
  res.status(200).send("News article deleted");
});

app.listen(3000, () => {
  console.log("News API listening on port 3000");
});
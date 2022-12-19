{-# LANGUAGE DisambiguateRecordFields #-}
{-# LANGUAGE OverloadedStrings        #-}
{-# LANGUAGE ScopedTypeVariables      #-}

module Search.Search where

import Data.Text (Text)
import Database.SQLite.Simple

import Search.Types

executeSearch :: Connection -> (Text -> IO ()) -> Text -> IO [Result]
executeSearch conn log q = do
    log $ "Searching " <> q
    query conn
        "SELECT url,title,snippet(pages,-1,'<b>','</b>','…',10) FROM pages WHERE pages MATCH ? ORDER BY rank ASC"
        (Only q)

{-# LANGUAGE OverloadedStrings #-}

import Hakyll

import Constants

main :: IO ()
main = hakyll $ do
    match "css/*" $ do
        route idRoute
        compile compressCssCompiler

    match "images/*" $ do
        route idRoute
        compile copyFileCompiler

    match "pages/*" $ do
        route   $ setExtension "html"
        compile $ pandocCompiler
            >>= loadAndApplyTemplate "templates/page.html" myContext
            >>= loadAndApplyTemplate "templates/default.html" myContext
            >>= relativizeUrls

    match "index.markdown" $ do
        route   $ setExtension "html"
        compile $ pandocCompiler
            >>= loadAndApplyTemplate "templates/default.html" myContext
            >>= relativizeUrls

    match "templates/*" $ compile templateCompiler

<?xml version="1.0"?>

<xsl:stylesheet version="1.0"
	exclude-result-prefixes="xspf"
	xmlns:xspf="http://xspf.org/ns/0/"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output
	encoding="iso-8859-1"
	method="text"
	omit-xml-declaration="yes"
	media-type="text/plain"/>

<xsl:template match="xspf:playlist">
 <xsl:apply-templates select="xspf:trackList"/>
</xsl:template>

<xsl:template match="xspf:trackList">
 <xsl:apply-templates select="xspf:track"/>
</xsl:template>

<xsl:template match="xspf:track">
 <xsl:value-of select="xspf:location"/>
 <xsl:if test="xspf:creator">
  <xsl:text>&#9;</xsl:text>
  <xsl:value-of select="xspf:creator"/>
 </xsl:if>
 <xsl:if test="xspf:title">
  <xsl:text>&#9;</xsl:text>
  <xsl:value-of select="xspf:title"/>
 </xsl:if>
 <xsl:if test="xspf:album">
  <xsl:text> [</xsl:text><xsl:value-of select="xspf:album"/><xsl:text>]</xsl:text>
 </xsl:if>
 <xsl:text>&#10;</xsl:text>
</xsl:template>

<xsl:template match="*"/>

</xsl:stylesheet>

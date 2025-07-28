<?xml version='1.0' encoding="iso-8859-1"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		version="1.0">
<xsl:output method="text"/>
<xsl:strip-space elements="*" />

<xsl:template match="text()" />

<xsl:template match="dir">
<xsl:value-of select="@name"/><xsl:text>;</xsl:text><xsl:value-of select="@length"/><xsl:text>;</xsl:text><xsl:value-of select="@atime"/><xsl:text>;</xsl:text><xsl:value-of select="@mtime"/><xsl:text>;</xsl:text><xsl:value-of select="@ctime"/>
<xsl:apply-templates/>
</xsl:template>

<xsl:template match="file">
<xsl:value-of select="@name"/><xsl:text>;</xsl:text><xsl:value-of select="@length"/><xsl:text>;</xsl:text><xsl:value-of select="@atime"/><xsl:text>;</xsl:text><xsl:value-of select="@mtime"/><xsl:text>;</xsl:text><xsl:value-of select="@ctime"/>

</xsl:template>
</xsl:stylesheet>

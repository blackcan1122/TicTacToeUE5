// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWindow.h"
#include "TreeGraphWidget.h"

/**
 * 
 */
class GEREDERT_API STreeGraphWindow : public SWindow
{
public:
    SLATE_BEGIN_ARGS(STreeGraphWindow) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
};